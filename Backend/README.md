# TxP

## Backend

<p align="center">
  <img height="150" src="https://upload.wikimedia.org/wikipedia/commons/thumb/d/d9/Node.js_logo.svg/2560px-Node.js_logo.svg.png" alt="neon.tech logo">
<img height="150" src="https://cf-assets.www.cloudflare.com/slt3lc6tev37/4WJkWMYGkEpa05B0hyL88E/91dd67e91752d39d94b60cdcdfdc287d/prismalogo-freelogovectors.net_.png" alt="prisma.io logo">
</p>

Our backend is made with NodeJS Express and Prisma

### Running the backend server

After cloning the project, remember to run:

```bash
npm install
```

To install the necessary dependencies.

And to install typescript

```bash
npm install typescript --save-dev
```

Before running the backend the environment values needs to be set. To do that create a file called `.env` in the root of the project and add the necessary values:

-   DATABASE_URL
-   DATABASE_URL_UNPOOLED

**Production**

To run the backend:

```bash
npx ts-node Backend/index.ts
```

You should get this message: "_REST API server ready at: http://localhost:4321_"

# API Specification

### /createUser

**Request**

```ts
username: string,
password: string
```

**Response**

```ts
Messages: "Success" | "Please fill out all required fields." | "Username already exists.";
```

### /login

**Request**

```ts
username: string,
password: string
```

**Response**

```ts
Messages: "Success" | "Please fill out all required fields." | "No user found." | "Incorrect password.";
```

### /deleteUser

**Request**

```ts
username: string,
password: string
```

**Response**

```ts
Messages: "Success" | "Please fill out all required fields." | "No user found." | "Incorrect password.";
```

### /users

**Request**

```ts
"No request / parameters needed";
```

**Response**

```ts
Messages: "List of every users data";
```
