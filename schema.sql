CREATE TABLE "workers" (
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    first_name VARCHAR(30) NOT NULL,
    last_name VARCHAR(30) NOT NULL
);

CREATE TABLE objects (
    id SMALLINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    title VARCHAR(100) NOT NULL,
    description VARCHAR(500) NULL
);

CREATE TABLE "keys" (
    id INTEGER PRIMARY KEY,
    worker_id INTEGER NOT NULL,
    expires_at TIMESTAMPZ NOT NULL,
    is_active BOOLEAN NOT NULL,
    CONSTRAINT "key_to_worker"
        FOREIGN KEY "worker_id"
        REFERENCES "workers" ("id")
);

CREATE INDEX "worker_keys" ON keys(worker_id);

CREATE TABLE "permissions" (
    worker_id INTEGER NOT NULL,
    object_id SMALLINT NOT NULL,
    PRIMARY KEY (worker_id, object_id),
    CONSTRAINT "permissions_object_id"
        FOREIGN KEY (object_id)
        REFERENCES objects(id)
        ON UPDATE CASCADE,
    CONSTRAINT permissions_worker_id
        FOREIGN KEY (worker_id)
        REFERENCES workers(id)
        ON UPDATE CASCADE
);

CREATE TYPE INTERACTION_TYPE AS ENUM ('ENTER', 'EXIT');
CREATE TYPE INTERACTION_FAILURE AS ENUM ('NOT_AUTHENTICATED', 'FORBIDDEN', 'NOT_FOUND', 'NOT_EXPECTED');

CREATE TABLE "interactions" (
    id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    worker_id INTEGER NULL,
    object_id SMALLINT NOT NULL,
    date_time TIMESTAMPTZ NOT NULL,
    interaction INTERACTION_TYPE NOT NULL,
    failure INTERACTION_FAILURE NULL,
    CONSTRAINT "interactions_worker_id"
        FOREIGN KEY (worker_id)
        REFERENCES "workers"("id")
        ON UPDATE CASCADE,
    CONSTRAINT "interactions_object_id"
        FOREIGN KEY (object_id)
        REFERENCES "objects"("id")
        ON UPDATE CASCADE,
    CONSTRAINT "interactions_consistency"
        CHECK ((failure = 'NOT_AUTHENTICATED') = (worker_id IS NULL))
);

CREATE INDEX interactions_with_object ON interactions(object_id, date_time);