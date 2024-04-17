// All the necessary libraries
import { PrismaClient } from "@prisma/client";
import express from "express";
const bcrypt = require("bcrypt");

// Initializing Prisma and Express
const prisma = new PrismaClient();
const app = express();

app.use(express.json());

// API ENDPOINT - /users
app.get("/users", async (req, res) => {
    const users = await prisma.user.findMany();
    res.json(users);
});

// API ENDPOINT - /createUser
app.post("/createUser", async (req, res) => {
    // Checks if input is received
    if (!req.body.username || !req.body.password) {
        return res.status(400).json("Please fill out all required fields.");
    }

    // Checks if user with with the input username exists
    const user = await prisma.user.findFirst({ where: { username: req.body.username } });
    if (user) {
        return res.status(400).json("Username already exists.");
    }

    // Hashing input password (with the help of bcrypt package)
    const password = bcrypt.hashSync(req.body.password, 10);

    // Creates new user in database with input username and hashed password of input password
    await prisma.user.create({
        data: { username: req.body.username, hashedPassword: password },
    });

    // Sends message "Success" when user is created in database
    res.status(201).json("Success");
});

// API ENDPOINT - /login
// REMEMBER TO SEND LOGIN TO ARDUIMO
app.post("/login", async (req, res) => {
    // Checks if input is received
    if (!req.body.username || !req.body.password) {
        return res.status(400).json("Please fill out all required fields.");
    }

    // Checks if user with with the input username exists
    const user = await prisma.user.findFirst({ where: { username: req.body.username } });
    if (!user) {
        return res.status(400).json("No user found.");
    }

    // Checks if password input matches with the hashed password in database
    if (!bcrypt.compareSync(req.body.password, user.hashedPassword)) {
        return res.status(400).json("Incorrect password.");
    }

    // Sends message "Success" when user is logged in
    res.status(201).json("Success");
});

// API ENDPOINT - /deleteUser
app.post("/deleteUser", async (req, res) => {
    // Checks if input is received
    if (!req.body.username || !req.body.password) {
        return res.status(400).json("Please fill out all required fields.");
    }

    // Checks if user with with the input username exists
    let user = await prisma.user.findFirst({ where: { username: req.body.username } });
    if (!user) {
        return res.status(400).json("No user found.");
    }

    // Checks if password input matches with the hashed password in database
    if (!bcrypt.compareSync(req.body.password, user.hashedPassword)) {
        return res.status(400).json("Incorrect password.");
    }

    // Deletes user where the username matches username input
    await prisma.user.delete({ where: { username: req.body.username } });

    // Sends message "Success" when user deleted
    res.status(201).json("Success");
});

// Starts app/backend on localhost:4321
app.listen(4321, () => console.log("REST API server ready at: http://localhost:4321"));
