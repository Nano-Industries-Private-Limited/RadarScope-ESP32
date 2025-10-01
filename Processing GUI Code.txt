import processing.serial.*;

Serial myPort;
String received;
float distance = 0;
float angle = 0;

float maxRange = 500.0; // cm (5m)
int centerX, centerY;
float scale;

void setup() {
  size(800, 420);
  background(0);
  
  centerX = width/2;
  centerY = height; // bottom center
  scale = (width/2 - 40) / maxRange;
  
  println(Serial.list());
  myPort = new Serial(this, "COM7", 115200); // replace with your port
  myPort.bufferUntil('\n');
  
  textSize(14);
}

void draw() {
  background(0);
  
  stroke(0, 255, 0);
  noFill();
  
  // Range rings + labels
  for (int r = 1; r <= 5; r++) {
    float dist_cm = r * 100;
    float rad = dist_cm * scale;
    arc(centerX, centerY, rad*2, rad*2, PI, TWO_PI);
    
    fill(0, 255, 0);
    noStroke();
    textAlign(CENTER, CENTER);
    text(r + "m", centerX-10, centerY-8 - rad);
    noFill();
    stroke(0, 255, 0);
  }
  
  // Vertical reference line (90°)
  stroke(0, 255, 0);
  line(centerX, centerY, centerX, centerY - maxRange*scale);
  
  // Radar origin
  fill(0, 255, 0);
  ellipse(centerX, centerY, 8, 8);
  
  // Draw target blip only if valid
  if (distance > 0 && distance <= maxRange) {
    float rad = radians(angle);
    float tx = centerX + cos(rad) * distance * scale;
    float ty = centerY - sin(rad) * distance * scale;
    
    fill(255, 0, 0);
    noStroke();
    ellipse(tx, ty, 10, 10);
  }
  
  // Telemetry
  fill(0, 255, 0);
  textAlign(LEFT, TOP);
  text("Distance: " + nf(distance/100.0, 1, 2) + " m", 10, 10);
  text("Angle: " + nf(angle, 1, 2) + "°", 10, 30);
}

void serialEvent(Serial p) {
  received = trim(p.readStringUntil('\n'));
  if (received != null) {
    String[] parts = split(received, ',');
    if (parts.length >= 3) {
      int valid = int(parts[0]);
      if (valid == 1) {
        distance = float(parts[1]); // cm
        angle = float(parts[2]);    // deg
      } else {
        // reset to zero if invalid
        distance = 0;
        angle = 0;
      }
    }
  }
}
