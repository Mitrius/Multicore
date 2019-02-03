FROM robbyjo/ubuntu-mkl:16.04-2018.1
COPY . /app
RUN apt-get update
RUN apt-get install -y build-essential
RUN apt-get install -y 
RUN apt-get install -y libboost-all-dev
WORKDIR /app