import { PrismaClient } from "@prisma/client";
import express from "express";
const bcrypt = require("bcrypt");

const prisma = new PrismaClient();
const app = express();

app.use(express.json());

app.get("/users", async (req, res) => {
    const users = await prisma.user.findMany();
    res.json(users);
});

app.post("/createUser", async (req, res) => {
    const username = req.body.username;
    let password = req.body.hashedPassword;

    let user = await prisma.user.findFirst({ where: { username: username } });
    if (user) {
        return res.status(400).json("Username already exists.");
    }

    if (!username || !password) {
        return res.status(400).json("Please fill out all required fields.");
    }
    const salt = await bcrypt.genSalt();
    password = await bcrypt.hash(password, salt);

    const newUser = await prisma.user.create({
        data: { username: username, hashedPassword: password },
    });
    res.status(201).json(newUser);
});

app.post("/login", async (req, res) => {
    const user = await prisma.user.findFirst({ where: { username: req.body.username } });
    if (!user) {
        return res.status(400).json("No user found.");
    }

    res.status(201).json(user);
});

app.listen(4321, () => console.log("REST API server ready at: http://localhost:4321"));
