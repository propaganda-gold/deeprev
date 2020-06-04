# Configuring Cassandra

This document discusses how to configure *Cassandra* on an *Ubuntu 18.04* instance, so that it can be used with *deeprev*.

## Check that Cassandra is Installed

First, make sure that you have installed Casssandra for deeprev. This should be done by the installation steps you have already run. However, for concreteness, the command to install Cassandra should be:

```bash
cd ~/deeprev/cpp
./install/cassandra.sh
```

## Create the Keyspace

In Cassandra, each database needs to be contained in a "keyspace". Keyspaces are relatively unique to Cassandra. Let us quote from them for completeness:

```
A keyspace is the top-level database object that controls the replication for the object it contains at each datacenter in the cluster. Keyspaces contain tables, materialized views and user-defined types, functions and aggregates. Typically, a cluster has one keyspace per application. Since replication is controlled on a per-keyspace basis, store data with different replication requirements (at the same datacenter) in different keyspaces. Keyspaces are not a significant map layer within the data model.
```

So, create a keyspace. We will use the name 'keyspace1' in the following.

```cassandra
CREATE KEYSPACE keyspace1
WITH replication = {'class':'SimpleStrategy', 'replication_factor' : 3};
```

## Create the Databases

We need the following databases:

* broadcast

Create a database using the following command:

```cassandra
USE keyspace1;
CREATE TABLE keyspace1.broadcast (id UUID PRIMARY KEY, pbtxt TEXT, creationTime TIMESTAMP);
```

