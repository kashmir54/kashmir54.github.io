#!/usr/bin/env python
import pymysql, urlparse, hashlib, pycurl, signal, base64, sys, os, re
from flask import Flask, Response, request, render_template, flash, g
from config import setup_db, db_conf, generate
from StringIO import StringIO
from PIL import Image

__author__ = 'makelaris'

app = Flask(__name__)
app.secret_key = generate(69)

setup_db() # Setup database structure

conn = pymysql.connect(**db_conf)
db = conn.cursor()

db.execute('SET GLOBAL connect_timeout=1')
db.execute('SET GLOBAL wait_timeout=4')
db.execute('SET GLOBAL interactive_timeout=4')

class LogRedir(object):
	redirects = 0

	def __init__(self, location):
		type(self).redirects += 1
		self.location = location

	def __iter__(self):
		return self.__dict__.iteritems()

	def toJSON(self):
		from flask.json import loads, dump
		return loads(dumps(self, default=lambda o: o.__dict__, sort_keys=True, ident=4))

def is_html(url):
	try:

		return_headers = StringIO()

		c = pycurl.Curl()
		c.setopt(c.URL, url)
		c.setopt(c.HEADERFUNCTION, return_headers.write)
		c.setopt(c.TIMEOUT, 10)

		resp = c.perform_rs()
		c.close()

	except pycurl.error as e:
		return flash('[*] {}'.format(e), 'danger')

	find_header = lambda r, h: {k.lower(): v.strip() for k, v in [line.split(':', 1) for line in r.getvalue().splitlines() if ':' in line]}.get(h, None)
	location_header = find_header(return_headers, 'location')

	if location_header:
		location = '[+] Did you mean %s ?' % location_header
		return flash(location.format(dest=LogRedir(location_header)), 'warning')

	if re.match('^<!doctype.*>', resp, flags=re.IGNORECASE) is not None:
		return True
	
	return False

class StderrLog(object):
	def close(self):
		pass

	def __getattr__(self, name):
		return getattr(sys.stderr, name)

from selenium.webdriver import PhantomJS

class Driver(PhantomJS):
	def __init__(self, *args, **kwargs):
		super(Driver, self).__init__(*args, **kwargs)
		self._log = StderrLog()

prepare_image = lambda f: 'data:image/png;base64,{}'.format(base64.b64encode(open(f).read()))

def serve_screenshot_from(url, domain, width=1000, min_height=400, wait_time=10):
	from selenium.webdriver.support.ui import WebDriverWait
	from selenium.webdriver.common.desired_capabilities import DesiredCapabilities

	dcap = dict(DesiredCapabilities.PHANTOMJS)
	dcap['phantomjs.page.settings.userAgent'] = ('HTB/1.0')

	driver = Driver(
		desired_capabilities=dcap, 
		service_log_path='/tmp/ghostdriver.log', 
		service_args=['--cookies-file=/tmp/cookies.txt', '--ignore-ssl-errors=true', '--ssl-protocol=any']
	)

	driver.set_page_load_timeout(wait_time)
	driver.implicitly_wait(wait_time)

	driver.set_window_position(0, 0)
	driver.set_window_size(width, min_height)

	driver.get(url)

	WebDriverWait(driver, wait_time).until(lambda r: r.execute_script('return document.readyState') == 'complete')

	screenshot = driver.get_screenshot_as_png()

	file_id = generate(14)
	filename = 'screenshots/{}.png'.format(file_id)

	region = Image.open(StringIO(screenshot)).crop((0, 0, width, min_height))
	region.save(filename, 'PNG', optimize=True, quality=95)

	driver.service.process.send_signal(signal.SIGTERM)
	driver.quit()

	if not os.path.exists(filename):
		flash('[*] Something went wrong during caching', 'warning')
		return render_template('index.html')

	screenshot = prepare_image(filename)

	db.execute('INSERT INTO screenshots (url, filename, integrity) VALUES (%s, %s, %s)', (url, filename, hashlib.sha256(screenshot[22:]).hexdigest()))

	flash('[+] Successfully cached {}'.format(domain), 'success')
	return locals()

def serve_cached_web(url, domain):
	db.execute('SELECT filename FROM screenshots WHERE url = %s ORDER BY created_at DESC', (url,))
	screenshot = prepare_image(db.fetchone().get('filename'))
	
	flash('[+] {} is already cached'.format(domain), 'warning')
	return locals()

def cache_web(url):
	domain = urlparse.urlparse(url).hostname

	db.execute('SELECT COUNT(filename) FROM screenshots WHERE url = %s AND TIMESTAMPDIFF(MINUTE, DATE(created_at), NOW()) >= 1 ORDER BY created_at DESC', (url,))

	if db.fetchone().itervalues().next():
		return serve_cached_web(url, domain)

	if is_html(url):
	 	return serve_screenshot_from(url, domain)

	return locals()

@app.route('/', methods=['GET', 'POST'])
def index():
	if request.method == 'POST':
		return render_template('index.html', cached=True, **cache_web(request.form.get('url', '')))

	return render_template('index.html')

@app.route('/debug')
def debug():
	return Response(open(__file__).read(), mimetype='text/plain')

if __name__ == '__main__':
	app.run('0.0.0.0', port=1337)