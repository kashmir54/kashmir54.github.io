from flask import Flask, Response, render_template, request, render_template_string

app = Flask(__name__)
app.config['SECRET_KEY'] = open('flag').read()

def reservation(name):
	HTML = '{{% extends "index.html" %}}{{% block content %}}'
	HTML += 'Thank you for registering, {name}</br>'
	HTML += '{{% endblock %}}'
	return HTML.format(name=name)

@app.route('/')
def index():
	name = request.args.get('name', '')
	if name:
		return render_template_string(reservation(name))
	return render_template('index.html')

@app.route('/debug')
def debug():
	return Response(open(__file__).read(), mimetype='text/plain')

if __name__ == '__main__':
	app.run(host='0.0.0.0', port=1337)