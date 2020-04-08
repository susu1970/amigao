note!<br>
first:<br>
	type "ulimit -s unlimited" to avoid segment fault(stack overflow)<br>
second:<br>
	enlarge the "%max_allowed_packet%" in mariadb(or mysql)<br>
third:<br>
	use nginx to proxy the website<br>
fourth:<br>
	use spawn-fcgi to start search_server<br>
others:<br>
	...<br>
