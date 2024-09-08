// server.js
const express = require('express');
const bodyParser = require('body-parser');
const formData = require('form-data');
const Mailgun = require('mailgun.js');
const path = require('path');

const app = express();

// Middleware to parse incoming JSON data
app.use(bodyParser.json());

// Serve static files from the 'public' directory
app.use(express.static(path.join(__dirname, 'public')));

// Handle GET request to the root route
app.get('/', function (req, res) {
    res.sendFile(path.join(__dirname, 'index.html'));
});

// Handle POST request (form submission)
app.post('/', function (req, res) {
    const email = req.body.email; // Get email from the JSON request

    console.log("Email: " + email);

    // Mailgun setup
    const mailgun = new Mailgun(formData);
    const mg = mailgun.client({
        username: 'api',
        key: process.env.MAILGUN_API_KEY || '1510cd0bea0843c4ac920b4727f168da-2b755df8-e6f67ba2' // Replace with your actual Mailgun API key
    });

    // Mailgun email sending
    mg.messages.create(' sandbox45b311254d144c998e1acfd2886d7c52.mailgun.org', {
        from: "Excited User <mailgun@your-mailgun-domain>",
        to: [email],
        subject: "Hello!",
        text: "Welcome to our mailing list!",
        html: "<h1>Welcome to our mailing list!</h1>"
    })
    .then(msg => {
        console.log("Email sent successfully: ", msg);
        res.send("Form submitted and email sent successfully!");
    })
    .catch(err => {
        console.error("Error sending email: ", err);
        res.send("There was an error sending the email.");
    });
});

// Start the server
app.listen(8000, function () {
    console.log("Server is listening on port 8000");
});
