#use this file to install docker, download mongoDB and zorba docker containers

wget -qO- https://get.docker.com/ | sh
sudo docker pull matthiasl64/ee_zorba:z3
sudo docker build -t tutum/mongodb .
sudo docker run -d -p 27017:27017 -p 28017:28017 -e AUTH=no tutum/mongodb
sudo docker run -it -p 80:80 matthiasl64/ee_zorba:z3
