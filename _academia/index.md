---
title: "Kashmir54 - Research"
categories: [academic]
date: 2023-03-10
comments: true
description: Academia research
image: /images/kashmir.png
favicon: /images/favicon.ico
---

# Research

During final days of the university, I worked on phishing detection, using several techniques and designing new ones to become one of the main problems on the social engineering attacks: phishing websites. I did other reasearch during that time, but not as relevant or were not complete. You can check more at my [ORCID](https://orcid.org/0000-0002-0788-916X)

My main contribution was the following article:

## Phishing websites detection using a novel multipurpose dataset and web technologies features

DOI: [https://doi.org/10.1016/j.eswa.2022.118010](https://doi.org/10.1016/j.eswa.2022.118010)

<div class="academia">
  <div class="article">
    <img src="/images/academia/eswa.png">
  </div>
  <div class="abstract">
    <p>Phishing attacks are one of the most challenging social engineering cyberattacks due to the large amount of entities involved in online transactions and services. In these attacks, criminals deceive users to hijack their credentials or sensitive data through a login form which replicates the original website and submits the data to a malicious server. Many anti-phishing techniques have been developed in recent years, using different resource such as the URL and HTML code from legitimate index websites and phishing ones. These techniques have some limitations when predicting legitimate login websites, since, usually, no login forms are present in the legitimate class used for training the proposed model. Hence, in this work we present a methodology for phishing website detection in real scenarios, which uses URL, HTML, and web technology features. Since there is not any updated and multipurpose dataset for this task, we crafted the Phishing Index Login Websites Dataset (PILWD), an offline phishing dataset composed of 134,000 verified samples, that offers to researchers a wide variety of data to test and compare their approaches. Since approximately three-quarters of collected phishing samples request the introduction of credentials, we decided to crawl legitimate login websites to match the phishing standpoint. The developed approach is independent of third party services and the method relies on a new set of features used for the very first time in this problem, some of them extracted from the web technologies used by the on each specific website. Experimental results show that phishing websites can be detected with 97.95% accuracy using a LightGBM classifier and the complete set of the 54 features selected, when it was evaluated on PILWD dataset.</p>
  </div>
</div>


### TL;DR

Using machine learning we built a model that uses URL, HTML code and the web technologies behind the website to determinate whether a website is phishing or not, reaching up to 97.95% accuracy. 

The main contributions are:

- We used legitimate login websites, which is way more difficult to differenciate from phishing websites. The rest of the articles in the state-of-the-art use normal index websites (which, for the most part, do not have any login form). This allows our algorithm to be used on real-world scenario.
- New HTML features are proposed, uplifting the detection performance.
- We used [Wappalyzer](https://www.wappalyzer.com/) to obtain the web technologies used on the websites. With them, we proposed several features that not only enhances the detection, but also makes the model more robust againts usual detection bypass techniques.
- Built the largest phishing dataset with a great amount of raw data: URL, HTML code, Screenshots, Web Tecnologies and an offline copy of the website resources. This allows other researchers to compare their work, no matter which tecnique or resource they use.
