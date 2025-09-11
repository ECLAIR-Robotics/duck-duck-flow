# 9-8-2025 Notes

Who was here? Robert, Dylan, Isabella, Shalini +2

Since most people are interested in software, we'll work on building the website, esoecially since parts are still on the way

# Software Setup
Install the follwing software, clone the codebase to your machine, and get the website running

install node.js
installed codebase
`git clone https://github.com/ECLAIR-Robotics/duck-duck-flow.git`
`cd duck-duck-flow/worker`
`npm install`

go to `package.json` and add `"build": "vite build",` like this:
  "scripts": {
    ...
    "build": "vite build",

create a `dist` folder in `worker`
do `npm run dev`
you should get

## Next time

try to get the test button to connect to the local database
how ot get permissions to insert into cloudflare database


