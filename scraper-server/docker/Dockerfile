# Use the official image as a parent image.
FROM debian:latest

ENV container docker

# Set the working directory.
WORKDIR /scraper-server

# Copy the rest of your app's source code from your host to your image filesystem.
COPY . .

# scraper-server works on 13337 port
EXPOSE 13337

RUN apt -y update
RUN apt -y install init cmake clang git pkg-config

# install dependencies to build vcpkg
RUN apt -y install curl zip unzip tar

# install vcpkg
RUN git clone https://github.com/Microsoft/vcpkg.git && \
./vcpkg/bootstrap-vcpkg.sh

# install scraper-server dependencies through vcpkg
RUN chmod +x /scraper-server/scripts/vcpkg-init-scripts/vcpkg-init-unix.sh && \
export PATH="/scraper-server/vcpkg:$PATH" && \
/scraper-server/scripts/vcpkg-init-scripts/vcpkg-init-unix.sh

# building scraper-server
RUN cmake -B build -DCMAKE_TOOLCHAIN_FILE=/scraper-server/vcpkg/scripts/buildsystems/vcpkg.cmake
RUN cmake --build build -j$(nproc)

# installing .service file for our sraper-server service
RUN cat /scraper-server/docker/scraper-server.service >> /etc/systemd/system/scraper-server.service

VOLUME [ "/sys/fs/cgroup" ]

RUN systemctl enable scraper-server

CMD [ "/sbin/init" ]
