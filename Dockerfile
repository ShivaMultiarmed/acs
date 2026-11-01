FROM ubuntu:22.04 AS build

RUN apt update \
    && apt install -y --no-install-recommends \
        build-essential cmake pkg-config \
        libpqxx-dev libcpp-httplib-dev nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /

COPY . /src

RUN cmake -S /src -B /build -DCMAKE_BUILD_TYPE=Debug && cmake --build /build -j$(nproc)

FROM build AS debug

RUN apt update \
    && apt install -y --no-install-recommends gdb gdbserver \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build

ENTRYPOINT ["gdbserver", ":8888", "/acs/build"]

FROM ubuntu:22.04 AS result

RUN apt update \
    && apt install -y --no-install-recommends \
      libpqxx-6.4 libpq5 libcpp-httplib-dev \
    && rm -rf /var/lib/apt/lists/*

COPY --from=build /build/acs /app/acs

WORKDIR /app

ENTRYPOINT ["./acs"]