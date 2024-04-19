// All the necessary libraries
import { PrismaClient } from "@prisma/client";
import express from "express";
const bcrypt = require("bcrypt");
require("dotenv").config();
const jwt = require("jsonwebtoken");
const https = require("https");

// Getting the jwtSecret from our .env file
const jwtSecret = process.env.jwtSecret;

// Initializing Prisma and Express
const prisma = new PrismaClient();
const app = express();

// Our generated certificate and key
/*const credentials = {
    key: process.env.serverKEY,
    cert: process.env.serverCRT,
};*/

// Creates https server
//const httpsServer = https.createServer(credentials, app);
app.use(express.json());

// Creating our JWT token
const maxAge = 3 * 24 * 60 * 60;
const createToken = (id: any) => {
    return jwt.sign({ id }, jwtSecret, {
        expiresIn: maxAge,
    });
};

// Creating our authentication using JWT to verify
const requireAuth = (req: any, res: any, next: any) => {
    // If there is no cookies return "Unauthorized"
    if (!req.headers.cookie) {
        return res.status(401).json("Unauthorized.");
    }

    // Splits our token so the encrypted JWT is accessible
    const token = req.headers.cookie.split("=")[1];

    // If there is a token verify with out jwtSecret to ensure it is ours
    if (token) {
        jwt.verify(token, jwtSecret, (err: any) => {
            if (err) {
                res.status(400).json("Unauthorized.");
            } else {
                next();
            }
        });
    } else {
        res.status(400).json("Unauthorized.");
    }
};

// API ENDPOINT - /users
app.get("/users", requireAuth, async (req, res) => {
    const users = await prisma.user.findMany();
    res.json(users);
});

// API ENDPOINT - /createUser
app.post("/createUser", async (req, res) => {
    // Checks if input is received
    if (!req.body.username || !req.body.password) {
        return res.status(406).json("Please fill out all required fields.");
    }

    // Checks if user with with the input-username exists
    const user = await prisma.user.findFirst({ where: { username: req.body.username } });
    if (user) {
        return res.status(400).json("Username already exists.");
    }

    // Hashing input-password (with the help of bcrypt package)
    const password = bcrypt.hashSync(req.body.password, 10);

    // Creates new user in database with input-username and hashed password of input-password
    await prisma.user.create({
        data: { username: req.body.username, hashedPassword: password },
    });

    // Finds the user and creates a token with the id of the user
    const findId = await prisma.user.findFirst({ where: { username: req.body.username } });
    const token = createToken(findId?.id);

    // Sends message "Success" and JWT cookie when user is logged in
    res.cookie("JWT", token, { maxAge: maxAge * 1000, secure: true, httpOnly: true });
    res.status(201).json("Success");
});

// API ENDPOINT - /login
// REMEMBER TO SEND LOGIN TO ARDUIMO
app.post("/login", async (req, res) => {
    // Checks if input is received
    if (!req.body.username || !req.body.password) {
        return res.status(406).json("Please fill out all required fields.");
    }

    // Checks if user with with the input-username exists
    const user = await prisma.user.findFirst({ where: { username: req.body.username } });
    if (!user) {
        return res.status(400).json("No user found.");
    }

    // Checks if input-password matches with the hashed password in database
    if (!bcrypt.compareSync(req.body.password, user.hashedPassword)) {
        return res.status(400).json("Incorrect password.");
    }

    // Finds the user and creates a token with the id of the user
    const findId = await prisma.user.findFirst({ where: { username: req.body.username } });
    const token = createToken(findId?.id);

    // Sends message "Success" and JWT cookie when user is logged in
    res.cookie("JWT", token, { maxAge: maxAge * 1000, secure: true, httpOnly: true });
    res.status(200).json("Success");
});

// API ENDPOINT - /deleteUser
app.post("/deleteUser", requireAuth, async (req, res) => {
    // Checks if input is received
    if (!req.body.username || !req.body.password) {
        return res.status(406).json("Please fill out all required fields.");
    }

    // Checks if user with with the input-username exists
    let user = await prisma.user.findFirst({ where: { username: req.body.username } });
    if (!user) {
        return res.status(400).json("No user found.");
    }

    // Checks if input-password matches with the hashed password in database
    if (!bcrypt.compareSync(req.body.password, user.hashedPassword)) {
        return res.status(400).json("Incorrect password.");
    }

    // Deletes user where the username matches input-username
    await prisma.user.delete({ where: { username: req.body.username } });

    // Sends message "Success" when user deleted
    res.status(200).json("Success");
});

// Starts app/backend on localhost:4321
app.listen(4321, () => console.log("REST API server ready at: https://localhost:4321"));
