
const char MAIN_page[] PROGMEM = R"=====(
<html>
  <head>
    <meta charset="utf-8">
    <title>My HTML Game</title>
  </head>
  <body onload="startGame()">
  
    <button onclick="startGame()"><strong>RESET</strong></button>
    <canvas id="myCanvas" style="border:5px solid black; background-color: rgb(20, 20, 20)"> </canvas>

    <script>

var blocksY = 10  ;  // defines how many blocks there will be, you can change this

function mapRange (value_to_map, in_min, in_max, out_min, out_max) {  //mapping function
    return (value_to_map - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

function startGame() { // this function is called when the page loads or when clicked on reset button


    var webSocket; //webSocket object

    webSocket = new WebSocket('ws://' + window.location.hostname + ':81/'); //connect to websockets
    webSocket.onmessage = function(event) {                                 // this function is called when a webSocket message arrives
        var data = JSON.parse(event.data);                                  // parsing the message using JSON
        platform.x = mapRange(data.value, 0, 1023, 0, 900);                 // using  the message data to control the position of the platform
    }





    gameArea.start(); // sets up the canvas element
    
   var blocksX = 8  ; //number of blocks in x direction, do not change this
   blocks = [];       //create bocks array

    //        create the blocks       //
   /////////////////////////////////////
   var i;
   var h;
   for(i = 0; i < blocksY; i++){    
       for(h = 0; h < blocksX; h++){     
        blocks.push( new component(100, 30, "green", 20 + (h * 115), 20 + (i * 35), 0, 0))

       }

   }

     //create the projectile object
   projectile = new component(30, 30, "magenta", Math.floor(Math.random() * 900) + 50, Math.floor(Math.random() * 180) + 300, 3, -3);  
   platform = new component(100, 25, "red", 450, 550, 0, 0);  //create the platform object
   top_ = new component(gameArea.canvas.width, 1, "black", 0, 0, 0, 0);                           //
   bot = new component(gameArea.canvas.width, 1, "black", 0, gameArea.canvas.height, 0, 0);       //   create the objects of the borders
   right = new component(1, gameArea.canvas.height, "black", gameArea.canvas.width, 0, 0, 0);     //
   left = new component(1, gameArea.canvas.height, "black", 0, 0, 0, 0);                          // 
    
}


////////////////////////////////////////////
//          object constructor            //
////////////////////////////////////////////
function component(width, height, color, x, y, x_speed, y_speed) {
  //variables for the object
    this.width = width;
    this.height = height;
    this.x = x;
    this.y = y;
    this.x_speed = x_speed;
    this.y_speed = y_speed;  
  //drawing the object on the canvas 
    this.update = function(){   
        ctx = gameArea.context;
        ctx.fillStyle = color;
        ctx.fillRect(this.x, this.y, this.width, this.height);
    }
  //checking for crashes
    this.crashWith = function(otherobj) {
        var myleft = this.x;
        var myright = this.x + (this.width);
        var mytop = this.y;
        var mybottom = this.y + (this.height);
        var otherleft = otherobj.x;
        var otherright = otherobj.x + (otherobj.width);
        var othertop = otherobj.y;
        var otherbottom = otherobj.y + (otherobj.height);
        var crash = true;
        if ((mybottom < othertop) ||
         (mytop > otherbottom) ||
         (myright < otherleft) ||
         (myleft > otherright)) {
          crash = false;
        }
            return crash;
      }
    
}

// geameArea object, sets up the canvas
var gameArea = { 
 canvas : document.getElementById("myCanvas"),
 start : function() {
    this.canvas.width = 1000;
    this.canvas.height = 600;
    this.context = this.canvas.getContext("2d");
    this.interval = setInterval(updateGameArea, 20);
    },
 clear : function() {
        this.context.clearRect(0, 0, this.canvas.width, this.canvas.height);
      },
 stop  : function() {
        clearInterval(this.interval);

 }


}


//this function is called every frame
function updateGameArea() {
    gameArea.clear();  // clear the previous frame


//checking for crashes with blocks in x direction
    projectile.x += projectile.x_speed ; //move projectile in x
  
   //check for crash with all blocks
    var check = 0;
    var g;
    for(g = 0; g < blocks.length; g++){
        if(blocks[g] != undefined){  // dont check with destroyed blocks
           if(projectile.crashWith(blocks[g])){ // if detected crash 
             projectile.y_speed *= -1;  // invert y speed(bounce)
             delete blocks[g]; // destroy the block

             check = 1;  // dont check for crashes in y direction
        }
      }
    }


// checking for crashes in y direction
    projectile.y += projectile.y_speed ;
    var k;
    for(k = 0; k < blocks.length; k++){
        if(blocks[g] != undefined){
    if(projectile.crashWith(blocks[k]) && check == 0 ){
     //   console.log("crashblock   Y");
     //   console.log(k);
     projectile.x_speed *= -1;
     delete blocks[k];
        }
      }
    }





    //draw the objects on the canvas
     projectile.update();
     platform.update();

    //draw the blocks on the canvas 
     var i;
     for(i = 0; i < blocks.length; i++){
        if(blocks[i] != undefined) { //dont draw destroyed blocks
            blocks[i].update();
        }
         
     }

    //draw the borders on the canvas
     top_.update();
     bot.update();
     right.update();
     left.update();
 
   //check for crashes with the platform
    if(projectile.crashWith(platform)){
        console.log("bounce");
        projectile.y_speed = -1 * projectile.y_speed; //bounce
    }
   //check for crashes with the borders
        if(projectile.crashWith(top_)){
            console.log("bounce_top");
            projectile.y_speed = -1 * projectile.y_speed; //bounce
        }

            if(projectile.crashWith(bot)){
                console.log("bounce_bot");
                gameArea.stop(); //stop the game if crash with bottom border detected
            }

                if(projectile.crashWith(right)){
                    console.log("bounce_right");
                    projectile.x_speed = -1 * projectile.x_speed; //bounce
                }

                    if(projectile.crashWith(left)){
                        console.log("bounce_left");
                        projectile.x_speed = -1 * projectile.x_speed; //bounce
    }
  }


   </script>
  </body>
</html>
)=====";
