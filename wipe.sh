docker stop ubuntu-container
docker rm ubuntu-container
docker image remove ubuntu20
docker build . -t ubuntu20 --no-cache