__author__ = 'creator'
'''
import urllib.request
from bs4 import BeautifulSoup



x= urllib.request.urlopen('http://minibb.nisamlab.com/index.php?')
y= urllib.request.urlopen('http://127.0.0.1/index.php?')
print(x.read())

soup = BeautifulSoup(x.read())

print (soup.prettify())

'''

'''
import urllib.request
import json

from urllib.parse import urljoin
from urllib.error import URLError
from urllib.error import HTTPError

class SampleLogin():

    def __init__(self, environment, username, password):
        self.environment = environment
        # Sample environment value can be: http://example.com
        self.username = username
        self.password = password

    def login(self):
        sessionUrl = urljoin(self.environment,'/path/to/resource/you/post/to')
        reqBody = {'username' : self.username, 'password' : self.password}
        # If you need encoding into JSON, as per http://stackoverflow.com/questions/25491541/python3-json-post-request-without-requests-library
        data = json.dumps(reqBody).encode('utf-8')

        headers = {}
        # Input all the needed headers below
        headers['User-Agent'] = "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2272.101 Safari/537.36"
        headers['Accept'] = "application/json"
        headers['Content-type'] = "application/json"

        req = urllib.request.Request(sessionUrl, data, headers)

        try:
            response = urllib.request.urlopen(req)
            return response
        # Then handle exceptions as you like.
        except HTTPError as httperror:
            return httperror
        except URLError as urlerror:
            return urlerror

s = SampleLogin( 'http://minibb.nisamlab.com/index.php?','han','a124')
print (s.req())
'''

import requests


url = 'http://minibb.nisamlab.com/index.php?'
payload={'mode':'login','pagetype':'index','user_pwd':'a123','user_usr':'han'}
header = {
'Accept':'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
'Accept-Encoding':'gzip, deflate',
'Accept-Language':'ko-KR,ko;q=0.8,en-US;q=0.5,en;q=0.3',
'Connection':'keep-alive',
'User-Agent':'Mozilla/5.0'
}

req = requests.Request('Post',url,headers=header,params=payload)

r=req.prepare()
s=requests.Session()
result = s.send(r)

print (result.text)
print (result.cookies)
print (requests.utils.dict_from_cookiejar(result.cookies))
print (result)

session = requests.session()
p = session.post(url, {'mode':'login','pagetype':'index','user_pwd':'a123','user_usr':'han'})
print ('headers', p.headers)
print ('cookies', requests.utils.dict_from_cookiejar(session.cookies))
print ('html',  p.text)


from bs4 import BeautifulSoup
import json

#soup = BeautifulSoup(r)
#print (soup.prettify())




'''
Accept
text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Encoding
gzip, deflate
Accept-Language
ko-KR,ko;q=0.8,en-US;q=0.5,en;q=0.3
Connection
keep-alive
Host
minibb.nisamlab.com
Referer
http://minibb.nisamlab.com/index.php?
User-Agent
Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:39.0) Gecko/20100101 Firefox/39.0

mode : login
pagetype : index
queryStr : ''
user_pwd : 1234
user_usr : admin
'''