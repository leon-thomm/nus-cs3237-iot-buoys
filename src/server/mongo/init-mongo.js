db.createUser(
    {
        user: "cs3237",
        pwd: "thisisag00dp4ssw0rd",
        roles: [
            {
                role: "readWrite",
                db: "cs3237"
            }
        ]
    }
),
db.createCollection("init")