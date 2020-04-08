note!
first:
	type "ulimit -s unlimited" to avoid segment fault(stack overflow)
second:
	enlarge the "%max_allowed_packet%" in mariadb(or mysql)
third:
	use nginx to proxy the website
fourth:
	use spawn-fcgi to start search_server
others:
	...
