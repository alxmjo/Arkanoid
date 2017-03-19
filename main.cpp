#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// constexpr defines an immutable compile-time value
constexpr int windowWidth{800}, windowHeight{600};
constexpr float ballRadius{10.0f}, ballVelocity{8.0f};

// Create constants for paddle
constexpr float paddleWidth{60.0f}, paddleHeight{20.0f}, paddleVelocity{6.0f};

struct Ball {
    // CircleShape is an SFML class that defines a renderable circle
    CircleShape shape;

    // 2D vector that stores the Ball's velocity
    Vector2f velocity{-ballVelocity, -ballVelocity};

    // Let's create the Ball constructor
    // argument posX -> starting x coordinate
    // argument posY -> starting y coordinate
    Ball(float posX, float posY) {
        // Apply position, radius, color, and origin to the CircleShape shape
        shape.setPosition(posX, posY);
        shape.setRadius(ballRadius);
        shape.setFillColor(Color::Red);
        shape.setOrigin(ballRadius, ballRadius);
    }

    // Update the ball, move its shape by the current velocity
    void update() {
        shape.move(velocity);

        // Keep ball inside screen

        // If leaving toward left, change horizontal velocity to positive (to the right)
        if (left() < 0) {
            velocity.x = ballVelocity;
        } else if (right() > windowWidth) {
            velocity.x = -ballVelocity;
        }

        // Top and bottom collisions
        if (top() < 0) {
            velocity.y = ballVelocity;
        } else if (bottom() > windowHeight) velocity.y = -ballVelocity;

    }

    // Create property methods to easily get commonly used values
    float x()       { return shape.getPosition().x; }
    float y()       { return shape.getPosition().y; }
    float left()    { return x() - shape.getRadius(); }
    float right()    { return x() + shape.getRadius(); }
    float top()    { return y() - shape.getRadius(); }
    float bottom()    { return y() + shape.getRadius(); }
};

struct Paddle {
    RectangleShape shape;
    Vector2f velocity;

    Paddle(float posX, float posY) {
        shape.setPosition(posX, posY);
        shape.setSize({paddleWidth, paddleHeight});
        shape.setFillColor(Color::Red);
        shape.setOrigin(paddleWidth / 2.0f, paddleHeight / 2.0f);
    }

    void update() {
        shape.move(velocity);

        // Use keyboard unput to change paddle position
        if (Keyboard::isKeyPressed(Keyboard::Key::Left) && left() > 0) {
            velocity.x = -paddleVelocity;
        } else if (Keyboard::isKeyPressed(Keyboard::Key::Right) && right() < windowWidth) {
            velocity.x = paddleVelocity;
        } else {
            velocity.x = 0;
        }


    }

    float x()       { return shape.getPosition().x; }
    float y()       { return shape.getPosition().y; }
    float left()    { return x() - shape.getSize().x / 2.0f; }
    float right()   { return x() + shape.getSize().x / 2.0f; }
    float top()     { return y() - shape.getSize().y / 2.0f; }
    float bottom()  { return y() + shape.getSize().y / 2.0f; }
};

// Template for testing if collisions are happening
template<class T1, class T2> bool isIntersecting(T1& mA, T2& mB) {
    return mA.right() >= mB.left() && mA.left() <= mB.right()
            && mA.bottom() >= mB.top() && mA.top() <= mB.bottom();
};

// Test collission between ball and paddle
void testCollision(Paddle& mPaddle, Ball& mBall) {
    // If no intersection, return
    if (!isIntersecting(mPaddle, mBall)) return;

    // Otherwise push the ball upwards
    mBall.velocity.y = -ballVelocity;

    // Direct it depending on where ball contacted paddle
    if (mBall.x() < mPaddle.x()) {
        mBall.velocity.x = -ballVelocity;
    } else {
        mBall.velocity.x = ballVelocity;
    }
}

int main() {
    // Create an instance of Ball positioned at the center of the window
    Ball ball {windowWidth / 2, windowHeight / 2};

    // Create paddle and center in window
    Paddle paddle{ windowWidth / 2, windowHeight - 50};

    // Creation of the game window
    RenderWindow window{{windowWidth, windowHeight}, "Arkanoid - 1"};
    window.setFramerateLimit(60);

    // Game loop
    while (true) {
        // Prevent freezing
        Event event;
        window.pollEvent(event);

        // Clear the window from previously drawn graphics
        window.clear(Color::Black);

        // If Escape is pressed, break out of the loop
        if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

        // Update the ball and paddle
        ball.update();
        paddle.update();

        // Test collision
        testCollision(paddle, ball);

        // Show the window contents
        window.draw(ball.shape);
        window.draw(paddle.shape);
        window.display();
    }

    return 0;
}
