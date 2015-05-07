sudo apt-get install php5;
sudo apt-get install php5-dev;
sudo apt-get install php5-cli;
sudo apt-get install libapache2-mod-php5;
sudo apt-get install php5-curl;
sudo apt-get install php5-gd;
sudo apt-get install php5-idn;
sudo apt-get install php-pear;
sudo apt-get install php5-imagick;
sudo apt-get install php5-imap;
sudo apt-get install php5-mcrypt;
sudo apt-get install php5-memcache;
sudo apt-get install php5-ps;
sudo apt-get install php5-pspell;
sudo apt-get install php5-recode;
sudo apt-get install php5-snmp;
sudo apt-get install php5-tidy;
sudo apt-get install php5-xmlrpc;
sudo apt-get install php5-xsl;
sudo apt-get install php5-common;
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv 7F0CEB10
sudo echo "deb http://downloads-distro.mongodb.org/repo/ubuntu-upstart dist 10gen" | tee -a /etc/apt/sources.list.d/10gen.list
sudo apt-get update -y
sudo apt-get install mongodb-10gen -y
