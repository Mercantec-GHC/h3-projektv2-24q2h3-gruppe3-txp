// All the necessary libraries
import { PrismaClient } from "@prisma/client";
import express from "express";
const bcrypt = require("bcrypt");
require("dotenv").config();
const jwt = require("jsonwebtoken");

// Getting the jwtSecret from our .env file
const jwtSecret = process.env.jwtSecret;

// Initializing Prisma and Express
const prisma = new PrismaClient();
const app = express();
app.use(express.json());

const port = process.env.PORT || 4321;

// Creating our JWT token
const maxAge = 3 * 24 * 60 * 60;
const createToken = (id: any) => {
    return jwt.sign({ id }, jwtSecret, {
        expiresIn: maxAge,
    });
};

// Creating our authentication using JWT to verify
const requireAuth = (req: any, res: any, next: any) => {
    // If there is no cookies return "No cookie"
    if (!req.headers.cookie) {
        return res.status(401).json("No cookie");
    }

    // Splits our token so the encrypted JWT is accessible
    const token = req.headers["cookie"]?.split("JWT=")[1];

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

    // Sends message "Success" when user is created
    res.status(201).json("Success");
});

// API ENDPOINT - /login
// REMEMBER TO SEND LOGIN TO ARDUIMO
app.post("/login", async (req, res) => {
    // Checks if input is received
    if (!req.body.username || !req.body.password) {
        return res.status(406).json("Please fill out all required fields.");
    }

    if (!req.body.arduinoDevice) {
        return res.status(406).json("Please send device name!");
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

    await prisma.sessions.update({ where: { ArduinoDevice: req.body.arduinoDevice }, data: { Account: token } });
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

// API ENDPOINT - /getUserById
app.post("/getUserById", requireAuth, async (req, res) => {
    // Splits our token so the encrypted JWT is accessible
    let token = req.headers["cookie"]?.split("JWT=")[1];

    // If there is a token verify with out jwtSecret to ensure it is ours
    if (token) {
        jwt.verify(token, jwtSecret, async (err: any, decodedToken: any) => {
            if (err) {
                res.status(400).json("Unauthorized.");
            } else {
                // Finds user where the id from the JWT matches with the id in the database
                let user = await prisma.user.findUnique({ where: { id: decodedToken.id } });

                // Sends user data
                res.status(200).json(user);
            }
        });
    }
});

// API ENDPOINT - /saveScore
app.post("/saveScore", requireAuth, async (req, res) => {
    let token = req.headers["cookie"]?.split("JWT=")[1];
    // If there is a token verify with out jwtSecret to ensure it is ours
    if (token) {
        jwt.verify(token, jwtSecret, async (err: any, decodedToken: any) => {
            if (err) {
                res.status(400).json("Unauthorized.");
            } else {
                // Finds user where the id from the JWT matches with the id in the database
                const user = await prisma.user.findUnique({ where: { id: decodedToken.id } });

                if (!user) {
                    res.status(404).json("User not found");
                    return;
                }

                const replay = await prisma.replay.create({ data: { userId: user.id, replay: "replay/file/path" } });

                const map = await prisma.map.create({ data: { userId: user.id } });

                if (!map) {
                    res.status(404).json("Map not found");
                    return;
                }

                const skin = await prisma.skin.create({ data: { skin_path: "skin/file/path", userId: user.id } });

                if (!skin) {
                    res.status(404).json("Skin not found");
                    return;
                }

                const game = await prisma.game.create({ data: { mapId: map.id, skinId: skin.id, gameId: req.body.gameId } });

                await prisma.score.create({
                    data: { userId: user?.id, score: req.body.score, gameId: game.gameId, replayId: replay.id },
                });
                res.status(201).json("Success");
            }
        });
    }
});

// API ENDPOINT - /getHighscores
app.get("/getHighscores", requireAuth, async (req, res) => {
    // Searches for top-score users by requested gameId (1 = snake, 2 = brick breaker, 3 = pong)
    const highscores = await prisma.score.findMany({
        where: { gameId: req.body.gameId },
        distinct: ["userId"],
        orderBy: { score: "desc" },
        include: { userRelation: { select: { username: true } } },
    });

    res.status(200).json(highscores);
});

// API ENDPOINT - /personalHighscore
app.post("/personalHighscore", requireAuth, async (req, res) => {
    // Searches for highscore from a user by requested gameId (1 = snake, 2 = brick breaker, 3 = pong)
    const highscore = await prisma.score.findMany({
        where: { AND: [{ gameId: req.body.gameId }, { userId: req.body.userId }] },
        distinct: ["userId"],
        orderBy: { score: "desc" },
    });
    res.status(200).json(highscore[0].score);
});

// API ENDPOINT - /sendArduinoName
app.post("/sendArduinoName", async (req, res) => {
    console.log(req.body);
    if (!req.body.arduinoDevice) {
        return res.status(406).json("Please send device name!");
    }

    if (await prisma.sessions.findFirst({ where: { ArduinoDevice: req.body.arduinoDevice } })) {
        return res.status(400).json("Arduino device name already exists.");
    }

    const arduino = await prisma.sessions.create({ data: { ArduinoDevice: req.body.arduinoDevice, Account: req.body.Token } });

    console.log(arduino);
    res.status(200).json(arduino);
});

// API ENDPOINT - /getDevices
app.get("/getDevices", async (req, res) => {
    const devices = await prisma.sessions.findMany();
    res.status(200).json(devices);
});

// API ENDPOINT - /getDevice/:ArduinoDevice
app.get("/getDevice/:arduinoDevice", async (req, res) => {
    if (!req.params.arduinoDevice) {
        return res.status(406).json("Please send device name!");
    }

    const device = await prisma.sessions.findFirst({ where: { ArduinoDevice: req.params.arduinoDevice } });
    res.status(200).json(device);
});

// Starts app/backend on localhost:4321
app.listen(port, () => console.log(`REST API server ready at: http://localhost:${port}`));
