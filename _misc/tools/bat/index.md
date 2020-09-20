---
title: "Tools - Bat"
categories: [tools]
date: 2020-09-16
hasComments: true
description: Tools - Bat.
---

# Bat

[Bat](https://github.com/sharkdp/bat) is a great tool for syntax highlighting and git integration on your shell. It can be used like cat for file display on the shell and I find it very usefull in order to show text files with non-printable characters, colored reports or to quick error checks thanks to the line number display:

<p align="center">
  <img src="/images/misc/tools/bat/1_linpeas.png" width="600"/>
</p>


Some python3 highlight:


<p align="center">
  <img src="/images/misc/tools/bat/1_highlight.png" width="600"/>
</p>

And also it supports integration with other commands like man, find, tail...

<p align="center">
  <img src="/images/misc/tools/bat/1_man.png" width="600"/>
</p>


## Instalation

You can read the full instructions on their [GitHub project](https://github.com/sharkdp/bat) but I can resume it here if you are looking into the Kali instalation:

- Go to their [release page](https://github.com/sharkdp/bat/releases) and download your _.deb_ file for Kali.

- Go to your downloads folder and run:

```
sudo dpkg -i bat_0.15.4_amd64.deb
```

If you want man highlight, you can set it up with the following command to set the enviroment variable:

```
export MANPAGER="sh -c 'col -bx | bat -l man -p'"
man 2 select
```

