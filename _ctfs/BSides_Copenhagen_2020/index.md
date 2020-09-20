---
title: "BSides Copenhagen 2020 - Writeup"
categories: [box]
date: 2020-09-03
hasComments: true
image: /images/walkthroughs/hackthebox/traceback/traceback_logo.png
description: "BSides Copenhagen 2020 - Writeup."
---
<br/><br/>
# Web

## BonechewerCon

The devil himself is enticing us to commit some SSTI feng shui, would you be interested in doing so?

We can see a coment with /debug, lets check that path:

<p align="center">
  <img src="/images/writeups/BSides Copenhagen 2020/Web/BonechewerCon/1_debug.png" width="600"/>
</p>

It shows this code: 

{% highlight python %}
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
{% endhighlight %}

We can see that it's formatting the variable name, which is a user input, without sanitizing it. We can go ahead and [exploit SSTI](https://medium.com/@nyomanpradipta120/ssti-in-flask-jinja2-20b068fdaeee) further [reading about bypasses](https://0day.work/jinja2-template-injection-filter-bypasses/):

<p align="center">
  <img src="/images/writeups/BSides Copenhagen 2020/Web/BonechewerCon/3_exploit.png" width="500"/>
</p>

As shown, we can exploit that vulnerability, so lets get the flag with the input: {{config['SECRET_KEY']}}

There we go:

<p align="center">
  <img src="/images/writeups/BSides Copenhagen 2020/Web/BonechewerCon/4_flag.png" width="600"/>
</p>

``` flag: HTB{r3s3rv4t1on_t0_h311_1s_a11_s3t} ```

<br/>
## baby interdimensional internet

aw man, aw geez, my grandpa rick is passed out from all the drinking again, where is a calculator when you need one, aw geez

<p align="center">
  <img src="/images/writeups/BSides Copenhagen 2020/Web/baby interdimensional internet/1_web.png" width="500"/>
</p>

Checking out the HTML we can see again the /debug comment, so lets check it out:

{% highlight python %}

from flask import Flask, Response, request, render_template, request
from random import choice, randint
from string import lowercase
from functools import wraps

app = Flask(__name__)

def calc(recipe):
	global garage
	garage = {}
	try: exec(recipe, garage)
	except: pass

def GCR(func): # Great Calculator of the observable universe and it's infinite timelines
	@wraps(func)
	def federation(*args, **kwargs):
		ingredient = ''.join(choice(lowercase) for _ in range(10))
		recipe = '%s = %s' % (ingredient, ''.join(map(str, [randint(1, 69), choice(['+', '-', '*']), randint(1,69)])))

		if request.method == 'POST':
			ingredient = request.form.get('ingredient', '')
			recipe = '%s = %s' % (ingredient, request.form.get('measurements', ''))

		calc(recipe)

		if garage.get(ingredient, ''):
			return render_template('index.html', calculations=garage[ingredient])

		return func(*args, **kwargs)
	return federation

@app.route('/', methods=['GET', 'POST'])
@GCR
def index():
	return render_template('index.html')

@app.route('/debug')
def debug():
	return Response(open(__file__).read(), mimetype='text/plain')

if __name__ == '__main__':
	app.run('0.0.0.0', port=1337)

{% endhighlight %}


We can see a _exec(recipe, garage)_ funcion, and it will asign to the 'ingredient' variable (what ever name we want) the result of 'measurement' (whatever we want). So we will place a name on 'ingredient' and **open('flag').read()** on the 'measurement' parameter. I'm using burp repeater for making the POST request with the parameters:

<p align="center">
  <img src="/images/writeups/BSides Copenhagen 2020/Web/baby interdimensional internet/2_post.png" width="650"/>
</p>

``` flag: HTB{n3v3r_trust1ng_us3r_1nput_ag41n_1n_my_l1f3} ```
<br/>
## babysql

Let's find out [this](https://stackoverflow.com/questions/5741187/sql-injection-that-gets-around-mysql-real-escape-string/12118602#12118602):

```
mysql_query('SET NAMES gbk');
\xbf\x27 O/**/R 1=1 /*

if (preg_match_all('/'. implode('|', array('in', 'or', 'and', 'set', 'file',)). '/i', $s, $matches)) die(var_dump($matches[0]));
    return mysqli_real_escape_string($d->conn(), $s);
$db->query("SELECT * FROM users WHERE password=('$pass') AND username=('%s')", 'admin');

"1 %4f%52 1=1"--

\xbf\x27')|| 1=1 /*

?id=1&&mid(pass,1,1)=(0x61);%00

1 UNION SELECT * FROM users u -- -

I couldn't get it.
```
<br/><br/>

# Crypto

## Weak RSA

We get an encrypted file and a public RSA key.

```
kali@kali:~/Desktop/BSides Copenhagen 2020/Crypto/Weak RSA$ /opt/RsaCtfTool/RsaCtfTool.py --dumpkey --key ./pubkey.pem 
private argument is not set, the private key will not be displayed, even if recovered.
n: 609983533322177402468580314139090006939877955334245068261469677806169434040069069770928535701086364941983428090933795745853896746458472620457491993499511798536747668197186857850887990812746855062415626715645223089415186093589721763366994454776521466115355580659841153428179997121984448771910872629371808169183
e: 387825392787200906676631198961098070912332865442137539919413714790310139653713077586557654409565459752133439009280843965856789151962860193830258244424149230046832475959852771134503754778007132465468717789936602755336332984790622132641288576440161244396963980583318569320681953570111708877198371377792396775817

kali@kali:~/Desktop/BSides Copenhagen 2020/Crypto/Weak RSA$ /opt/RsaCtfTool/RsaCtfTool.py --createpub -n 609983533322177402468580314139090006939877955334245068261469677806169434040069069770928535701086364941983428090933795745853896746458472620457491993499511798536747668197186857850887990812746855062415626715645223089415186093589721763366994454776521466115355580659841153428179997121984448771910872629371808169183 -e 387825392787200906676631198961098070912332865442137539919413714790310139653713077586557654409565459752133439009280843965856789151962860193830258244424149230046832475959852771134503754778007132465468717789936602755336332984790622132641288576440161244396963980583318569320681953570111708877198371377792396775817 > key.pub

kali@kali:~/Desktop/BSides Copenhagen 2020/Crypto/Weak RSA$ /opt/RsaCtfTool/RsaCtfTool.py --publickey ./key.pub --private > key
```

And the you can use the private key to decrypt it, or you can go straight forward with the tool:

```
kali@kali:~/Desktop/BSides Copenhagen 2020/Crypto/Weak RSA$ /opt/RsaCtfTool/RsaCtfTool.py --publickey ./key.pub --uncipherfile ./flag.enc 
private argument is not set, the private key will not be displayed, even if recovered.

[*] Testing key ./key.pub.
Can't load roca because sage is not installed
Can't load ecm2 because sage is not installed
Can't load qicheng because sage is not installed
Can't load smallfraction because sage is not installed
Can't load ecm because sage is not installed
Can't load boneh_durfee because sage is not installed
[*] Performing siqs attack on ./key.pub.
[!] Warning: Modulus too large for SIQS attack module
[*] Performing cube_root attack on ./key.pub.
[*] Performing primefac attack on ./key.pub.
[*] Performing pastctfprimes attack on ./key.pub.
[*] Performing partial_q attack on ./key.pub.
[*] Performing noveltyprimes attack on ./key.pub.
[*] Performing wiener attack on ./key.pub.

Results for ./key.pub:

Unciphered data :
HEX : 0x0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004854427b6231365f655f356d346c6c5f645f337175346c355f7733316e33725f34373734636b7d
INT (big endian) : 2357392946640097540211419645858895520810208771688666037754674144921765688545923475455795751805
INT (little endian) : 22546574263662123411523113128044215139929103295256955398886722465953163007000026640058669241937130344552566514396588140080195580660405015242202668398374037246444419998235653404182362721762856885981820839225967889839162532460437518669392406390949281459764025707163465962902177848023467525069105766717294567751680
STR : b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00HTB{b16_e_5m4ll_d_3qu4l5_w31n3r_4774ck}'
```

``` flag: HTB{b16_e_5m4ll_d_3qu4l5_w31n3r_4774ck} ```

<br/><br/>
# Forensic

## Clang 

We got a pcap file with RTP protocol... 

<p align="center">
  <img src="/images/writeups/BSides Copenhagen 2020/Forensics/Clang/1_rtp.png" width="500"/>
</p>

Let's try to retrieve any streaming in the packets with Wireshark tools:

Go to menu:
_Telephony > RTP > RTP Streams_
We can see 4 sequences, some are the phone holding ring, and one of those (the first one) what a sequence with the flag. Here we are opening the menu:

<p align="center">
  <img src="/images/writeups/BSides Copenhagen 2020/Forensics/Clang/2_tele.png" width="500"/>
</p>

Now select the fragment you want to analyze and hit _Analyze_, then hit _Play Streams_ now you will see the transmission, play it and you will hear the flag:

<p align="center">
  <img src="/images/writeups/BSides Copenhagen 2020/Forensics/Clang/3_flag.png" width="500"/>
</p>

``` flag: HTB{3242459345} ```

Thank for reading!

