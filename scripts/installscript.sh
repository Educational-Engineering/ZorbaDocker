sudo apt-get update
sudo apt-get install vim -y
sudo apt-get install build-essential cmake -y
sudo apt-get install apache2  -y
sudo apt-get install php5 libapache2-mod-php5 -y
sudo apt-get install php5 -y
sudo apt-get install php5-dev -y
sudo apt-get install php5-cli -y
sudo apt-get install libapache2-mod-php5 -y
sudo apt-get install php5-curl -y
sudo apt-get install php5-gd -y
sudo apt-get install php5-idn -y
sudo apt-get install php-pear -y
sudo apt-get install php5-imagick -y
sudo apt-get install php5-imap -y
sudo apt-get install php5-mcrypt -y
sudo apt-get install php5-memcache -y
sudo apt-get install php5-ps -y
sudo apt-get install php5-pspell -y
sudo apt-get install php5-recode -y
sudo apt-get install php5-snmp -y
sudo apt-get install php5-tidy -y
sudo apt-get install php5-xmlrpc -y
sudo apt-get install php5-xsl -y
sudo apt-get install php5-common  -y
sudo apt-get install bzr -y
sudo apt-get install git -y
sudo apt-get install libxml2-dev -y
sudo apt-get install uuid-dev -y
sudo apt-get install libicu-dev -y
sudo apt-get install libxerces-c-dev -y
sudo apt-get install libxslt-dev -y
sudo apt-get install libcurl4-openssl-dev -y
sudo apt-get install libxerces-c-dev -y
sudo apt-get install swig  -y
sudo apt-get install openjdk-6-jdk -y
sudo apt-get install libtidy-dev -y
sudo apt-get install libgeos-dev -y
sudo apt-get install imagemagick -y
sudo apt-get install libmagick++-dev -y
sudo apt-get install libmagickwand-dev -y
sudo apt-get install libc-client2007e -y
sudo apt-get install libc-client2007e-dev -y
sudo apt-get install libaccess-bridge-java-jni -y
sudo apt-get install mysql-client -y
sudo debconf-set-selections <<< 'mysql-server mysql-server/root_password password chaesfondue'
sudo debconf-set-selections <<< 'mysql-server mysql-server/root_password_again password chaesfondue'
sudo apt-get install mysql-server -y
sudo apt-get install libmysql-java -y
sudo apt-get install libboost-all-dev -y
sudo apt-get install flex -y
sudo apt-get install bison -y
sudo apt-get install python -y
sudo apt-get install scons -y
sudo apt-get install git -y
sudo apt-get install subversion -y

#no mongo needed in docker
#sudo apt-key adv --keyserver keyserver.ubuntu.com --recv 7F0CEB10
#sudo echo "deb http://downloads-distro.mongodb.org/repo/ubuntu-upstart dist 10gen" | tee -a /etc/apt/sources.list.d/10gen.list
#sudo apt-get update -y
#sudo apt-get install mongodb-10gen -y
###sudo apt-get install -y mongodb-org


sudo git clone git://github.com/mongodb/mongo-cxx-driver.git mongo-cxx-driver
sudo git checkout legacy
cd mongo-cxx-driver
sudo scons --prefix=/usr/local install
cd ..

#checkout specific revision because PHP support was deprecated in newer versions
sudo git clone git://github.com/28msec/zorba.git zorba
cd zorba
sudo git checkout ead13679f627ccc6241140b6080b0a941e5fc968
cd ..
sudo git clone git://github.com/28msec/zorba-csv-module.git zorba_modules/csv
sudo git clone git://github.com/28msec/zorba-jdbc-module.git zorba_modules/jdbc
sudo git clone git://github.com/28msec/zorba-data-cleaning-module.git zorba_modules/data-cleaning
sudo git clone git://github.com/28msec/zorba-data-formatting-module.git zorba_modules/data-formatting
sudo git clone git://github.com/28msec/zorba-excel-module.git zorba_modules/excel
sudo git clone git://github.com/28msec/zorba-info-extraction-module.git zorba_modules/info-extraction
sudo git clone git://github.com/28msec/zorba-html-module.git zorba_modules/html
sudo git clone git://github.com/28msec/zorba-http-client-module.git zorba_modules/http-client
sudo git clone git://github.com/28msec/zorba-oauth-module.git zorba_modules/oauth
sudo git clone git://github.com/28msec/zorba-process-module.git zorba_modules/process
sudo git clone git://github.com/28msec/zorba-system-module.git zorba_modules/system
sudo git clone git://github.com/28msec/zorba-util-jvm-module.git zorba_modules/util-jvm
sudo git clone git://github.com/28msec/zorba-schema-tools-module.git zorba_modules/schema-tools
sudo git clone git://github.com/28msec/zorba-stack-module.git zorba_modules/stack
sudo git clone git://github.com/28msec/zorba-queue-module.git zorba_modules/queue
sudo git clone git://github.com/28msec/zorba-couchbase-module.git zorba_modules/couchbase
sudo git clone git://github.com/28msec/zorba-sqlite-module.git zorba_modules/sqlite
sudo cp -r ZorbaDocker/mongo2/ zorba_modules/mongo2/

sudo mkdir zorba/build
cd zorba/build

sudo cmake -D CMAKE_INSTALL_PREFIX=/usr/local ..
sudo make
sudo make install

cd swig/python/
sudo cp _zorba_api.so _zorba_api_python.so
cd ..
cd ..
cd swig/php/
sudo cp libzorba_api.so libzorba_api_php.so
cd ..
cd ..
cd swig/ruby/
sudo cp zorba_api.so zorba_api_ruby.so
cd ..
cd ..

sudo cmake -D CMAKE_INSTALL_PREFIX=/usr/local ..
sudo make
sudo make install

cd ..
cd ..

#install php mongo support
printf "\n" | sudo pecl install apc

sudo cp ZorbaDocker/scripttesting/confs/php.ini /etc/php5/apache2/php.ini
sudo cp ZorbaDocker/scripttesting/confs/mongod.conf /etc/mongod.conf

#sudo service mongod restart
sudo /etc/init.d/apache2 restart

sudo git clone https://github.com/Educational-Engineering/ZorbaEditor.git /var/www/html/editor
