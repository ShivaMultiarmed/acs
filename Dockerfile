FROM ubuntu:22.04 AS build

RUN apt update
RUN apt install -y --no-install-recommends \
    build-essential cmake pkg-config \
    libpqxx-dev libpq-dev

WORKDIR /

COPY . /src

RUN cmake -S /src -B /build -DCMAKE_BUILD_TYPE=Debug
RUN cmake --build /build -j$(nproc)

FROM ubuntu:22.04 AS result

RUN apt update
RUN apt install -y --no-install-recommends \
    libpqxx-6.4 libpq5

COPY --from=build /build/acs /app/acs

WORKDIR /app

ENTRYPOINT ["acs"]