from Crypto.Cipher import AES
import random
import time
import base64
​
BIT_SIZE = 256
BYTE_SIZE = 32
​
def pad(payload, block_size=BYTE_SIZE):
    length = block_size - (len(payload) % block_size)
    return payload + chr(length) * length
​
def keygen():
    random.seed(BYTE_SIZE)
    h = random.getrandbits(BIT_SIZE)
    for i in range(BIT_SIZE):
        random.seed(time.time())
        h = h ^ random.getrandbits(2*BIT_SIZE/BYTE_SIZE)
    return hex(h)[2:-1]
​
def encrypt(data, key1, key2):
    cipher = AES.new(key1, mode=AES.MODE_ECB)
    ct = cipher.encrypt(pad(data))
    cipher = AES.new(key2, mode=AES.MODE_ECB)
    ct = cipher.encrypt(ct)
    return ct
​
def decrypt(ct, key1, key2):
    cipher = AES.new(key2, mode=AES.MODE_ECB)
    pt1 = cipher.decrypt(ct)
    cipher = AES.new(key1, mode=AES.MODE_ECB)
    pt2 = cipher.decrypt(pt1)
    return pt2
​
if __name__ == "__main__":
   
    #message = [REDUCTED]
    #flag = [REDUCTED]
​
    key1 = keygen()
    key2 = keygen()
    
    key1 = key1.decode('hex')
    key2 = key2.decode('hex')
​
    ct_message = encrypt(message, key1, key2)
    ct_flag = encrypt(flag, key1, key2)
​
    with open('packet_6.txt.enc', 'w') as f:
        f.write(base64.b64encode(ct_message))

    with open('flag.txt.enc', 'w') as f:
        f.write(base64.b64encode(ct_flag))