To sum up:

Positions (5, 7, 8, 13) to (0, 1, 3, 7):
'011010100100111' -> '000011011110011'

Once we change the chunk, we introduce it into the library and try to solve the error using hamming code. REMEMBER, this lib uses 16 bit chunks, we need to insert the remaining parity bit at position 0!!

We compare the output with the expected first letter, in this case the one of the flag 'n'.

This workflow is implemented on the following script along with the usage of previous hamming lib:


{% highlight python %}

from hamming import decode as dec
from bitarray import bitarray
import time
import binascii
import itertools

data_str = '011010100100111010010011110010110110110010010010011100001111101101101000110100110000010011100010001001110000110111100100110111111110101000101011011010000111100001000111001110010111001101011100011101111011100111111000101001110000011101011110010111111110001101110000011010010011101010010110101010001000011100101110100000101110000010110010100010101111010110001101001100001000101100100011111000100001110001100110001110101011010001111001111001101001110000110000111011001000010110001001010111111010101100010011011001110110111001100111101001001100110100100110001000101010111011101010110000111001011100111001'

# For dividing in chunks
def chunks(l, n):
  for i in range(0, len(l), n):
    yield l[i:i+n]


def text_from_bits(bits, encoding='utf-8', errors='surrogatepass'):
  n = int(bits, 2)
  return n.to_bytes((n.bit_length() + 7) // 8, 'big').decode(encoding, errors) or '\0'


# Pick the bits from the combination and insert them into the right position for parity bits
def swap(bits, comb):
  for i, j in zip([0,1,3,7], [0,1,2,3]):
    ex = bits.pop(comb[j])
    bits.insert(i, ex)
  return bits


# Divide in chunks of 15 bits
chunk_list = list(chunks(data_str, 15))
secret_key = []

# Get all permutations of length 4 with the 15 posible positions:
iterable = [x for x in range(0,14)]
combinations = list(itertools.permutations(iterable, r=4))
print('Total permutations: ', len(list(combinations)))

# Try all permutations that could lead into the right first two bytes
for combination in combinations:

  final_data = bitarray()

  for item in chunk_list[:6]:

    item = swap(list(item), combination)
    
    '''
    Add the first parity bit so the lib can get the 16 bits needed. 
    We know there is one wrong, therefore we will apply it the other way around.
    '''
    if item.count('1') % 2 == 0:
      item.insert(0,'1')
    else:
      item.insert(0,'0')

    data = bitarray(''.join(item))
    '''
    Get the result from fixing error:
    '''
    final_data.extend(dec(data))

  msg = ''.join(final_data.decode({'1':bitarray('1'), '0':bitarray('0')}))
  if msg[:48] == '011011100110000101100011011101000110011001111011':
    print(msg[:48], '= 011011100110000101100011011101000110011001111011')
    print('Combination = ', combination)
    secret_key = combination
    break

# The we could find 2 combinations, but only the first showed a reasonable flag.
# We stop when find the first permutation with the previous break instruction.

# Now that we know the permutations, retrieve the flag
final_data = bitarray()

for item in chunk_list:
  item = swap(list(item), secret_key)
  
  if item.count('1') % 2 == 0:
    item.insert(0,'1')
  else:
    item.insert(0,'0')
  data = bitarray(''.join(item))
  final_data.extend(dec(data))

msg = ''.join(final_data.decode({'1':bitarray('1'), '0':bitarray('0')}))
n = text_from_bits(msg, encoding='utf-8', errors='surrogatepass')
print(n)

{% endhighligh %}


After execution we can get out flag :D

```
kali@kali:~/Desktop/CTFs/NACTF/Crypto/Error2/hamming$ python3 script.py 
Total permutations:  24024
011011100110000101100011011101000110011001111011 = 011011100110000101100011011101000110011001111011
Combination =  (5, 7, 8, 13)
nactf{err0r_c0rr3cti0n_w1th_th3_c0rr3ct_f1le_q73xer7k9}
```

---

# General

##  Dr. 
J's Vegetable Factory #1 🥕
50

After years of collecting plush vegetable toys, Dr. J decided to take on his true passion: starting a vegetable factory. Dr. J is incredibly organized, so he likes all of his vegetables to be in the proper order. In fact, he built a robot "Turnipinator-1000" to alphabetize his vegetables for him! Unfortunately, Dr. J doesn't know what instructions to give Turnipinator-1000. Can you help him out? 🥬🥕🌽🍆🥦🥒🥑🍄

nc challenges.ctfd.io 30267

Give instructions in the form of numbers separated by spaces. Entering the number x will swap the vegetable in position x with the vegetable in position x+1. Positions start at zero, not one. (Dr. J is a programmer after all.) For example, given the following vegetables: Avocado, Brocolli, Eggplant, Daikon Radish, Carrot, one possible solution is "3 2 3"

Avocado, Brocolli, Eggplant, Daikon Radish, Carrot

    (swap 3 and 4)

Avocado, Brocolli, Eggplant, Carrot, Daikon Radish

    (swap 2 and 3)

Avocado, Brocolli, Carrot, Eggplant, Daikon Radish

    (swap 3 and 4)

Avocado, Brocolli, Carrot, Daikon Radish, Eggplant


The problem is like [bubble sort algorithm](https://www.toptal.com/developers/sorting-algorithms). The following algorithm retrieves the information from the server, gets the list, sortit alphabet-wise and gets the positions of each sorted element. Then on the original unsorted list we place the position it has to reach. Then the bubble sort algorithm does the trick, and each position change it does is collected into a list. Then the list is the input for the challenge.

This algorithm can be used in the next challenge, only change the option in the send line:

```

r.send("1\n") 

```


{% highlight python %}

from pwn import *
import re
import time

def bubbleSort(arr): 

    n = len(arr) 
    sequence = []
    for i in range(n-1): 
        for j in range(0, n-i-1): 
            if arr[j] > arr[j+1] : 
                sequence.append(str(j))
                arr[j], arr[j+1] = arr[j+1], arr[j] 
    return sequence


def get_sequence(current_list):

    unordered = current_list.split(', ')
    sorted_list = sorted(unordered)

    for desired_index, item in enumerate(sorted_list):
        current_index = unordered.index(item)
        unordered[current_index] = desired_index

    sequence = bubbleSort(unordered) 
    print('Sequence: ', ' '.join(sequence))
    return ' '.join(sequence)


r = remote('challenges.ctfd.io', 30267)
received = r.recv().decode('utf-8')
print(received)
r.send("1\n")
stage = 0
while True:
   
    msg = r.recv().decode('utf-8')
    
    print("-----------")
    print(msg)
    print("-----------")
    if 'nactf' in msg:
        print(msg)
        break
    if not stage:
        re_obj = re.search(r'\n\n(.*)\n\n', msg)
        vege = re_obj.groups(1)[0]
    else:
        re_obj = re.search(r'STAGE [0-9] 🥬🥕🌽🍆🥦🥒🥑🍄\n(.*)\n', msg)
        vege = re_obj.groups(1)[0]
    print("+++++")
    print(str(vege))
    print("+++++")
    sequence = get_sequence(vege)
    r.send("{}\n".format(sequence))
    stage += 1

{% endhighlight %}


Execute it and retrieve the flag:

``` nactf{1f_th3r3s_4_pr0b13m_13ttuce_kn0w_db4d736fd28f0ea39ec} ```

##  Dr. J's Vegetable Factory #2 🥕
150

Dr. J expanded his vegetable factory! Now he's got hundreds of vegetables. Same problem as last time: can you give Turnipinator-1000 the right instructions to sort Dr. J's vegetables? 🥬🥕🌽🍆🥦🥒🥑🍄

nc challenges.ctfd.io 30267

Use previous algorithm and change the option in the send line:

```

r.send("2\n") 

```


``` nactf{d0n7_w0rry_p34_h4ppy_f27ae283dd72cb62f685} ```

##  Dr. J's Vegetable Factory #3 🥕
175

Rahul hates vegetables. Rahul hates vegetables so much that he snuck into Dr. J's factory at night to sabotage Dr. J's vegetable production! He brought a sledgehammer and broke the wheels of Dr. J's robot! 😓 Now the robot is stuck in place, and instead of being able to swap any adjacent elements, it can only swap the elements in positions 0 and 1!

But Dr. J won't let this incident stop him from giving the people the vegetables they deserve! Dr. J is a problem solver 🧠. He organized his vegetables in a circle, and added a conveyor-belt that allows him shift the positions of the vegetables. He thinks that the conveyor belt should make it possible to sort his vegetables, but he's not 100% sure. Can you help him out?

nc challenges.ctfd.io 30267

Enter letters separated by spaces to sort Dr. J's vegetables. Entering "c" will activate the conveyor belt and shift all vegetables left one position. Entering "s" will swap the vegetable in position 0 with the vegetable in position 1.

The same as before, but now we fix the positions at 1 and 0, adding the conveyor belt function (pop first element and append it).

When position 0 is better than 1, swap, else, move conveyor. The exception case is if position 0 is the final element and the position 1 is the fisrt element, in this case, we move the conveyor.


{% highlight python %}
from pwn import *
import re
import time

def bubbleSort(arr, sorted_arr): 

    n = len(arr) 
    sequence = []
    bot, top = min(sorted_arr), max(sorted_arr)
    # While the array is not sorted...
    while arr != sorted_arr: 
        # Exceptional case
        if arr[0] == top and arr[1] == bot:
            # Move conveyor-belt
            first = arr.pop(0)
            arr.append(first)
            sequence.append('c')
            continue
        # Swap elements if 0 is greater than 1
        if arr[0] > arr[1]: 
            sequence.append('s')
            arr[0], arr[1] = arr[1], arr[0] 
        else:
            # Move conveyor-belt    
            first = arr.pop(0)
            arr.append(first)
            sequence.append('c')

    return sequence


def get_sequence(current_list):

    unordered = current_list.split(', ')
    sorted_list = sorted(unordered)

# Set objectives and arrays
    for desired_index, item in enumerate(sorted_list):
        current_index = unordered.index(item)
        unordered[current_index] = desired_index
    sorted_numbers = sorted(unordered)
    sequence = bubbleSort(unordered, sorted_numbers) 
    return ' '.join(sequence)


r = remote('challenges.ctfd.io', 30267)
received = r.recv().decode('utf-8')
print(received)
r.send("3\n")
stage = 0
while True:
   
    msg = r.recv().decode('utf-8')
    print("-----------")
    print(msg)
    print("-----------")
    if 'nactf' in msg:
        break
    if not stage:
        re_obj = re.search(r'order:\n\n(.*)', msg)
        vege = re_obj.groups(1)[0]
    print("+++++")
    print(str(vege))
    print("+++++")
    sequence = get_sequence(vege)
    r.send("{}\n".format(sequence))
    stage += 1
{% endhighlight %}


``` nactf{1t_t4k35_tw0_t0_m4n90_8a51c7b47fbe227} ```


##  Dr. J's Vegetable Factory #4 🥕
350

Thanks to you solving Dr. J's conveyor belt conundrum, Dr. J's vegetable factory is up and running once again. But this time, Juliet breaks into the factory to finish what Rahul started. For some reason, she doesn't break Turnipinator-1000 completely. First, she spins Turnipinator-1000 in a random direction. Then, she pulls the arms of Turnipinator-1000 a random distance apart so it can no longer swap adjacent elements! Now, it can only swap the vegetable in position x with the vegetable in position y!

Initially, Dr. J is very sad. 😥 Even with the conveyor belt, he doesn't think he can sort his vegetables anymore. But then, Dr. J has a brilliant revelation! 💡 He scribbles on scratch paper for a few minutes, then he gets up and removes a few of the vegetables from the conveyor belt.

Proudly, he declares, "Dr. J's Vegetable Factory is back in business!"

nc challenges.ctfd.io 30267

Same instructions as in part #3. Enter letters separated by spaces to sort Dr. J's vegetables. Entering "c" will activate the conveyor belt and shift all vegetables left one position. Entering "s" will swap the vegetable in position x with the vegetable in position y. (x and y will be given.)

Nah. I couldn't get it :(



That's all I could do! Thanks for reading!