__author__ = 'creator'

import requests



r = requests.post('192.168.32.12/login.php/',)

while(1):
    for i in range (1,8):
        r = requests.post('192.168.32.12/login_check.php/',)



import urllib

urllib.parse.urlopen()