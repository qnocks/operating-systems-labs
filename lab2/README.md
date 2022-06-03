# OS #2: Working with Text Streams in the Bash Shell

## Objective of the work
Learning the principles of working with the GNU / Linux command interpreter and the basics of processing text files using the `grep`, `awk`, `sed` commands.

## Reference material
For help on the bash shell, see [bash_help.pdf](bash_help.pdf). You can view the documentation for the shell command you are interested in by running `man <command_name>` in the console.

## Task
1.  Prepare the GNU/Linux operating system for work. If necessary, use the virtualization tool VMware Workstation / Player, [Oracle VirtualBox] (https://www.virtualbox.org/) or others. It is recommended to take [Ubuntu] (https://www.ubuntu.com/download) as a distribution kit /desktop).
2.  Create the directory `lab1` in the user's home directory `/home/user/` using the `mkdir` command. Change to this directory with the `cd` command. All further work is carried out inside this directory.
3.  Use the command `git clone <path_to_repository_on_github>` to create a local copy of the repository. Go to the directory with a copy of the repository (when cloning the repository, the directory will have the same name as the repository itself on github.com).
4.  Download archive with DNS server logs using `wget https://github.com/markpolyak/datasets/raw/main/data/dns-tunneling.log.tar.bz2` or `curl -L -O https:/ /github.com/markpolyak/datasets/raw/main/data/dns-tunneling.log.tar.bz2`. If your OS does not have the `wget` and `curl` utilities, install them yourself. On Ubuntu, this can be done with the commands `sudo apt-get install wget` and `sudo apt-get install curl` respectively. In this case, it is highly recommended to run `sudo apt-get update` before calling `sudo apt-get install ...`.
6.  Use the `tar -xjf dns-tunneling.log.tar.bz2` command to unzip the `dns-tunneling.log` file.
7.  Familiarize yourself with the format of the `dns-tunneling.log` file and the data stored in it using the commands `cat`, `head`, `tail`, `more`, `less`, etc.
8.  Count the number of entries in the `dns-tunneling.log` file.
9.  Calculate the number of the task option as the remainder of dividing the student's serial number according to the list in the journal by the number of task options. If the remainder is zero, you must take the last task.
10. Declare the `TASKID` variable in the `lab1.sh` file, specifying the number of the task variant as an integer as its value.
11. Declare the `VAR_1` variable in the `lab1.sh` file, specifying the number of entries in the `dns-tunneling.log` file as its value (this value must be calculated dynamically during the script execution).
12. Add code to the `lab1.sh` file for processing data from the `dns-tunneling.log` file in accordance with the task option using only the `bash` command interpreter commands. Use of other interpreted or compiled languages is prohibited. The script must read the `dns-tunneling.log` file and all results must be calculated based on it in real time, it is forbidden to use pre-obtained values in the script.
13. Make the files `lab1.sh` and `test.sh` executable with the `chmod` command.
14. Debug the `lab1.sh` script and make sure that the tests in `test.sh` run without errors. Keep in mind that in case of forced interruption of the tests (for example, by Ctrl + C command from the keyboard, or by turning off the computer power), you should restore the `dns-tunneling.log` file from the backup using the `mv dns-tunneling.log` command. bak dns-tunneling.log` or `git checkout HEAD dns-tunneling.log`.
15. Use the `git add lab1.sh`, `git commit` and `git push` commands to add the file with the written code to the repository. Verify that the test in the repository passed successfully.
16. Prepare and defend a lab report.


## Description of text data
The `dns-tunneling.log` file contains the [DNS-server](https://en.wikipedia.org/wiki/Domain_Name_System) logs, presented as a text file, in which each line corresponds to a record about the request received by the server. The following query parameters are stored in the logs, separated by a tab character:

1.	Telecom provider name: `character array`,
2.	The name of the node where the data is stored: `character array`,
3.	Request sequence number: `long`,
4.	The timestamp when the request came in: two `long` numbers separated by a dot; the first number is the number of seconds since January 1, 1970; the second number is the number of microseconds; those. in fact it is a `float` data type,
5.	User IP address: `character array`,
6.	User port: `int`,
7.	The local IP address where the request came from: `character array`,
8.	Local port: `int`,
9.	DNS server hardware name: `character array`,
10.	Request class: `int`,
11.	[Request type](https://en.wikipedia.org/wiki/List_of_DNS_record_types): `int`,
12.	Return code: `int`,
13.	Flags: `int`,
14.	Auxiliary identifier: `int`,
15.	Requested URL: `character array`,
16.	Zone: `character array`,
17.	Auxiliary field 1: `character array`,
18.	Auxiliary field 2: `character array`,
19.	Auxiliary field 3: `character array`,
20.	Auxiliary field 4: `character array`,
21.	Server response: `character array`,
22.	Auxiliary field 5: `character array`,
23.	Auxiliary field 6: `character array`,
24.	Response length: `int`

## Variants

In all variants, it is assumed that at the end of each line with text there is a newline character `\n`, and there are no empty lines.  

1.	Write a script using grep, sed, awk (you must use at least one of the indicated utilities; you do not need to use all three) to convert the data into XML format with the following structure:
```
<dnslog>
<row>
    <serial>Sequence number of the request</serial>
    <client_ip>user IP address</client_ip>
    <url>Requested URL</url>
</row>
<row>
    <serial>Sequence number of the request</serial>
    <client_ip>user IP address</client_ip>
    <url>Requested URL</url>
</row>
<row>
    ...
</row>
</dnslog>
```
The text inside the `<serial>`, `<client_ip>` and `<url>` tags must be replaced with the corresponding values from the logs. Use a tab character for indentation (4 spaces in the example above = one tab). Save the result of applying the written script to the last 15 lines of the `dns-tunneling.log` file in the `results.txt` file.  
In the `VAR_2` variable, write the number of entries in the resulting text file that contain access to any subdomains `1yf.de.` and `2yf.de.`.

2.	Write a script using grep, sed, awk (you must use at least one of the indicated utilities; you do not need to use all three) to convert the data into XML format with the following structure:
```
<dnslog>
<row>
    <timestamp>Timestamp when the request was made</timestamp>
    <client_ip>user IP address</client_ip>
    <client_port>User port</client_port>
</row>
<row>
    <timestamp>Timestamp when the request was made</timestamp>
    <client_ip>user IP address</client_ip>
    <client_port>User port</client_port>
</row>
<row>
    ...
</row>
</dnslog>
```
The text inside the `<timestamp>`, `<client_ip>` and `<client_port>` tags must be replaced with the corresponding values from the logs. Use a tab character for indentation (4 spaces in the example above = one tab). Save the result of applying the written script to the first 20 lines of the `dns-tunneling.log` file in the `results.txt` file.  
In the `VAR_2` variable, write the number of entries in the resulting text file that contain requests from users with IP addresses from the `10.1.*.*` subnet.

3.  Generate a list of IP addresses of users present in the logs, remove duplicates, sort the list in order of increasing IP address and save it in the `results.txt` file. In the `VAR_2` variable, write the number of requests that came from the IP address of the user who turned out to be the second in the `results.txt` file.
4.  Display in the format `"YYYY-MM-DD HH:mm:SS.ns"` the date and time of the earliest and latest DNS queries, where `DD` is the day, `MM` is the month, `YYYY` - year, `HH` - hour, `mm` - minutes, `SS` - seconds, `ns` - nanoseconds. To do this, use the `date` command with the `–d` flag, for example: `date -d @946684800`. Convert the time and date to the Coordinated Universal Time (UTC) time zone. Store the specified two dates in the `results.txt` file, each on a new line. When running the job on MacOS, it is recommended to use the `gdate` (GNU date) utility instead of `date`. When processing data, assume that the entries in the log may not be sorted in ascending order of the timestamp.  
    In the `VAR_2` variable, record the number of requests that came from the same user IP address as the earliest found request.
5.	Find the number of requests received during the first, second, third and fourth seconds of logging. Save the resulting four numbers to the `results.txt` file. When processing data, assume that the entries in the log may not be sorted in ascending order of the timestamp.
     In the `VAR_2` variable, write the port of the user from which the earliest found request came.
6.  Find all [second-level domain names](https://en.wikipedia.org/wiki/Second-level_domain_name) (including the top-level domain, i.e. `wikipedia.org`, not `wikipedia`) accessed by users for the duration of the log. Count the number of requests for each of the second-level domain names (including their subdomains). All domain names must first be converted to lower case. Output a table to the `results.txt` file, in which each line looks like:
    `<second-level domain name><tab character><number of requests for this domain name>`
    Sort the lines in the file in descending order by the number of requests to second-level domains. If the number of requests to multiple second-level domains is the same, sort them alphabetically by domain name.
    In the `VAR_2` variable, write the number of lines in the resulting `results.txt` file.
7. Count the number of hits to the `whatsapp.net.` and `google.com.` domains. Find the total amount of DNS traffic to each of these domains. The amount of traffic in one DNS request is equal to the number of characters that make up the requested name, including all subdomains and dots. Find the longest requested URL ever logged, not limited to `whatsapp.net.` and `google.com.` domains. If there is more than one URL of maximum length, take the first one in order of appearance in the logs. Form the file `results.txt` from three lines in the following form:
```
<longest requested URL>
whatsapp.net.;<number of hits to whatsapp.net.>;<total volume of DNS traffic to whatsapp.net.>
google.com.;<number of hits to google.com.>;<total volume of DNS traffic to google.com.>
```  
Write the total amount of DNS traffic to `google.com.` into the `VAR_2` variable.

8.	Write a script using `grep`, `sed`, `awk` (at least one of these utilities must be used; all three are optional) to convert data into JSON format with the following structure:
```
{
    "dnslog": [
        "entry": {
            "timestamp": "Timestamp when the request was made",
            "client ip": "user IP address",
            "url": "Requested URL"
        },
        "entry": {
            "timestamp": "Timestamp when the request was made",
            "client ip": "user IP address",
            "url": "Requested URL"
        },
        ...
    ]
}
```
The Russian text in quotation marks must be replaced with the corresponding values from the logs. In JSON format, all keys and all string values ​​must be placed in double quotes, while numeric values ​​(int, float) should be inserted without quotes. Use a tab character for indentation (4 spaces in the example above = one tab).
Save the result of applying the written script to lines 15 to 30 (inclusive) of the `dns-tunneling.log` file in the `results.txt` file.
In the `VAR_2` variable, write the number of entries in the resulting text file that contain access to any subdomains `1yf.de.` and `2yf.de.`.

9.	Write a script using `grep`, `sed`, `awk` (at least one of these utilities must be used; all three are optional) to convert data into JSON format with the following structure:
```
{
    "Sequence number of the request": {
        "timestamp": "Timestamp when the request was made",
        "client ip": "user IP address",
        "client port": "User port"
    },
    "Sequence number of the request": {
        "timestamp": "Timestamp when the request was made",
        "client ip": "user IP address",
        "client port": "User port"
    },
    ...
}
```
The Russian text in quotation marks must be replaced with the corresponding values from the logs. In JSON format, all keys and all string values ​​must be placed in double quotes, while numeric values (int, float) should be inserted without quotes. Use a tab character for indentation (4 spaces in the example above = one tab).
Save the result of applying the written script to lines 30 to 50 (inclusive) of the `dns-tunneling.log` file in the `results.txt` file.
In the `VAR_2` variable, write the number of entries in the resulting text file that contain requests from users with IP addresses from the `10.1.*.*` subnet.

10.	The [second-level](https://ru.wikipedia.org/wiki/Second-level_domain_name) domains `1yf.de.` and `2yf.de.` are DNS tunnels. They are used to transfer traffic over the DNS protocol bypassing HTTP (for example, to bypass blocking). Count the number of hits to each of the second-level domains `1yf.de.` and `2yf.de.` (this includes hits to all their subdomains). Calculate the average number of requests per second for all DNS tunnels for the first half hour of logging, rounded up to three decimal places (the decimal separator is a dot). Store the resulting numbers in the `results.txt` file, one per line: the first line is the number of hits to the domain `1yf.de.`, the second line is the number of hits to `2yf.de.`, the third line is the average number of hits to ` 1yf.de.` and `2yf.de.` for the first half hour. Note that the entries in the `dns-tunneling.log` file may appear in any order.  
       In the `VAR_2` variable, write the average number of requests per second across all DNS tunnels for the first half hour of logging.
11.	Count the number of unique users using DNS tunnels (see previous option). Users can be identified by their IP addresses. Determine the number of DNS queries made by each of the found users. Output a table to the `results.txt` file, in which each line looks like:
       `<IP address><tab character><number of queries using DNS tunnels made from the specified IP address>`  
       Sort the lines in the file in descending order by the number of requests to DNS tunnel domains. Users with the same number of requests are additionally sorted in ascending order of IP address. To do this, you can, for example, use re-sorting with the `-s` or `--stable` key.
       Write the number of lines in the `results.txt` file to the `VAR_2` variable.

12.	Calculate as a percentage how many DNS requests in the given log fall on DNS tunnels, broken down by `1yf.de.` and `2yf.de.` (see previous options), as well as `whatsapp.net.` domains and `google.com.`. The resulting numbers are rounded up to 3 decimal places and written in floating point format (the separator of the integer and fractional parts is a dot) to the `results.txt` file, one per line. The order of the numbers must be the same as indicated in the task.
       In the `VAR_2` variable, write the total number of DNS queries to `google.com.`, including subdomains.

13. Count the number of DNS queries of all types. Output a table to the `results.txt` file, in which each line looks like:  
    `<request type>;<number of requests of this type>`
    Sort the lines in the file in descending order by the number of requests of different types. If the number of requests of two different types is the same, sort them by request type number in ascending order.
    In the `VAR_2` variable, write the number of requests [`MX`](https://ru.wikipedia.org/wiki/Record_MX) (the code of this type of request is 15) in the resulting `results.txt` file.

14. Find all DNS queries of type `A` (query type code is 1). Output a table to the `results.txt` file, in which each line looks like:  
    `<domain name to which a request of type A is addressed><tab character><number of requests for this domain name>`
    Sort the lines in the file in descending order by the number of requests to domains. If the number of requests to multiple domains is the same, sort them alphabetically by domain name. All domain names must first be converted to lower case.
    In the `VAR_2` variable, write the number of lines in the resulting `results.txt` file.

15. Find all DNS requests to non-existent domains - [`NXDOMAIN`](http://www.iana.org/assignments/dns-parameters/dns-parameters.xhtml#dns-parameters-6) (return code is 3) . Find all unique IP addresses of users who made DNS requests to non-existent domains, and count the number of such requests from each of the users who made at least one such request. Output a table to the `results.txt` file, in which each line looks like:  
    `<IP address of the user who made at least one request to a non-existent domain><tab character><number of requests to non-existent domains from this user>`
    Sort the lines in the file in descending order by the number of requests. Users with the same number of requests are additionally sorted in ascending order of IP address. To do this, you can, for example, use re-sorting with the `-s` or `--stable` key.
    Write the number of lines in the `results.txt` file to the `VAR_2` variable.

16. Among all successful DNS queries - [`No Error`](http://www.iana.org/assignments/dns-parameters/dns-parameters.xhtml#dns-parameters-6) (return value code is 0) - count the number of requests of each type, see the "request type" field. Output a table to the `results.txt` file, in which each line looks like:
    `<request type><tab character><number of successful requests of this type>`
    Sort the lines in the file in descending order by the number of requests of different types. If the number of requests of two different types is the same, sort them by request type number in ascending order.
    Write the number of requests to the `VAR_2` variable [`AAAA`](http://www.iana.org/assignments/dns-parameters/dns-parameters.xhtml#dns-parameters-4) (the code of this request type is 28) in the resulting `results.txt` file.

17. Find all users sending DNS queries from odd ports less than `40000`. Store in `results.txt` a list of the IP addresses of these users, one address per line, sorted in ascending order of IP address.
    In the `VAR_2` variable, write the number of queries that came to the DNS server `10.208.61.116` from the IP address that appeared on the second line in the `results.txt` file.

18. Find all users who sent requests from even ports in the range from `1900` to `2000`, not including them. Output to the `results.txt` file a table in which each line looks like:
    `<user IP address><tab character><user port><tab character><number of requests from this port>`
    Sort the lines in the file in descending order by the number of requests. If the number of requests is the same, sort by IP address in ascending order. If the number of requests and the user's IP address match, additionally sort by port number in ascending order. To do this, you can, for example, use re-sorting with the `-s` or `--stable` key.
    In the `VAR_2` variable, write the number of requests that came to from the IP address that turned out to be the last one in the `results.txt` file.

19. Among the last 200 lines in the `dns-tunneling.log` file, find those whose requested URL starts with `aD`. Sort all found unique request URLs alphabetically and write them to the `results.txt` file.
    In the `VAR_2` variable, write the number of characters in the shortest line in the `results.txt` file.

20. Find all [second-level](https://en.wikipedia.org/wiki/Second-level_domain_name) domain names  located in the `net.` domain zone. Count the number of requests to each of the found second-level domains, including its subdomains. Output a table to the `results.txt` file, in which each line looks like:
    `<domain name><tab character><number of requests for this domain name>`
    Sort the lines in the file in descending order by the number of requests to domains. If the number of requests to multiple domains is the same, sort them alphabetically by domain name. All domain names must first be converted to lower case.
    In the `VAR_2` variable, write the number of requests to the second-level domain, which appeared on the third line in the `results.txt` file.
