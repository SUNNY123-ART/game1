import pygame
import sys
import time
import random

# Initialize Pygame
pygame.init()

# Set the size of the Pygame window
screen = pygame.display.set_mode((800, 600))

# Set the color of the player, the bullets, and the shapes (RGB)
player_color = (255, 255, 255)  # white
bullet_color = (0, 0, 0)  # black
red_ball_color = (255, 0, 0)  # red
green_ball_color = (0, 255, 0)  # green

# Set the player's initial HP
player_hp = 10

# Set the shapes' initial HP
red_ball_hp = 5
green_ball_hp = 5

# Set the player's position
player_pos = [400, 550]

# Set the shapes' positions and HP
red_balls = [[random.randint(0, 800), 0, red_ball_hp]]
green_balls = [[random.randint(0, 800), 0, green_ball_hp]]

# Set the bullets' positions
bullets = []

# Set the score
score = 0

# Set the time of the last red and green ball spawn
last_red_spawn = time.time()
last_green_spawn = time.time()

# Game loop
while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
                # Shoot a bullet
                bullets.append(player_pos.copy())
        elif event.type == pygame.MOUSEMOTION:
            # Move the player with the mouse
            player_pos[0] = pygame.mouse.get_pos()[0]

    # Fill the screen with a colorful background
    screen.fill((0, 255, 255))  # cyan

    # Draw the player with a "gun" (represented by a rectangle here)
    pygame.draw.rect(screen, player_color, pygame.Rect(player_pos[0], player_pos[1], 50, 50))

    # Spawn red balls every 20 seconds
    if time.time() - last_red_spawn > 20:
        red_balls.append([random.randint(0, 800), 0, red_ball_hp])
        last_red_spawn = time.time()

    # Spawn green balls every 5 seconds
    if time.time() - last_green_spawn > 5:
        green_balls.append([random.randint(0, 800), 0, green_ball_hp])
        last_green_spawn = time.time()

    # Move and draw the red balls
    for ball in red_balls:
        ball[1] += 1  # Move the ball down
        pygame.draw.circle(screen, red_ball_color, ball[:2], 50)

    # Move and draw the green balls
    for ball in green_balls:
        ball[1] += 1  # Move the ball down
        pygame.draw.circle(screen, green_ball_color, ball[:2], 50)

    # Move and draw the bullets
    for bullet in bullets:
        bullet[1] -= 5  # Move the bullet up
        pygame.draw.rect(screen, bullet_color, pygame.Rect(bullet[0], bullet[1], 5, 10))

    # Check for collisions between bullets and balls
    for bullet in bullets:
        for ball in red_balls:
            if pygame.Rect(bullet[0], bullet[1], 5, 10).colliderect(pygame.Rect(ball[0]-50, ball[1]-50, 100, 100)):
                ball[2] -= 1  # Decrease the ball's HP
                bullets.remove(bullet)  # Remove the bullet
                if ball[2] <= 0:
                    red_balls.remove(ball)  # Remove the ball
                    score += 1  # Increase the score
                break
        for ball in green_balls:
            if pygame.Rect(bullet[0], bullet[1], 5, 10).colliderect(pygame.Rect(ball[0]-50, ball[1]-50, 100, 100)):
                ball[2] -= 1  # Decrease the ball's HP
                bullets.remove(bullet)  # Remove the bullet
                if ball[2] <= 0:
                    green_balls.remove(ball)  # Remove the ball
                    score += 1  # Increase the score
                break

    # Check for collisions between balls and the player
    for ball in red_balls + green_balls:
        if pygame.Rect(player_pos[0], player_pos[1], 50, 50).colliderect(pygame.Rect(ball[0]-50, ball[1]-50, 100, 100)):
            player_hp -= 2  # Decrease the player's HP
            if player_hp <= 0:
                print("You died!")
                player_hp = 10  # Reset the player's HP

    # Draw the score and health bar
    font = pygame.font.Font(None, 36)
    score_text = font.render("Score: " + str(score), 1, (10, 10, 10))
    screen.blit(score_text, (20, 20))
    health_text = font.render("HP: " + str(player_hp), 1, (10, 10, 10))
    screen.blit(health_text, (20, 60))

    # Update the display
    pygame.display.flip()
