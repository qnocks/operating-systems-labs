#!/usr/bin/env bash

# edit the code below and add your code
TASKID=2
VAR_1=$(cat dns-tunneling.log | wc -l)

FILE=results.txt

echo '<dnslog>' > $FILE

awk '{print "<row>\n\t<timestamp>" $4 "</timestamp>\n\t<client_ip>" $5 "</client_ip>\n\t<client_port>" $6 "</client_port>\n</row>"}' dns-tunneling.log | head -n 100 >> $FILE

echo '</dnslog>' >> $FILE

VAR_2=$(grep -e "^.*<client_ip>10.1\..*\..*" $FILE | wc -l)
