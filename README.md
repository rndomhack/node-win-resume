# win-resume
Resume from sleep on windows

## Install
```sh
npm i --save win-resume
```

## Example
This example resume after 1 minutes.

```js
const Resume = require("win-resume");

let resume = new Resume({time: -60});   // Specify in seconds or Date object

resume.on("end", () => console.log("resume!"));

resume.start();
```
