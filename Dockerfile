FROM php:8.2-fpm

RUN apt-get update && apt-get install -y \
    build-essential gcc make locales zip jpegoptim optipng pngquant gifsicle vim unzip git curl cmake \
    libpng-dev libjpeg62-turbo-dev libfreetype6-dev

RUN apt-get clean && rm -rf /var/lib/apt/lists/*

# Install extensions
RUN pecl install xdebug-3.2.1 && docker-php-ext-enable xdebug
RUN docker-php-ext-install pdo_mysql

COPY ./ /var/www/
WORKDIR /var/www/formatter/

RUN cmake .
RUN make

WORKDIR /var/www/