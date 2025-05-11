---
title: Project Report
---

# Project Title

Object-Oriented Chess Game in C++

# Group Members

1\. Abdullah -- 24K-0822  
2. Hassan Ahmed -- 24K-1008  
3. Aazmeer -- 24K-0978

# Submission Date

April 24, 2025

# 1. Executive Summary {#executive-summary}

• Overview: This project aimed to develop a chess game using
object-oriented programming in C++. The project involved designing a
board, implementing each chess piece's behavior through classes,
handling player turns, and integrating an AI opponent using the
Stockfish engine.  
• Key Findings: The team successfully developed a functional chess game
with an intuitive console and GUI, integrated using SFML. Core OOP
concepts such as inheritance, polymorphism, and encapsulation were
effectively applied to manage the complexity of the game logic.

# 2. Introduction {#introduction}

• Background: Chess involves complex rules and movement logic, making it
an ideal project to showcase object-oriented design. Implementing this
game provided an opportunity to apply theoretical OOP knowledge
practically.  
• Project Objectives: To build a working chess game that supports two
players and integrates an AI opponent, while applying OOP principles to
create clean, modular, and maintainable code.

# 3. Project Description {#project-description}

• Scope: The project includes board initialization, player interaction
through a console-based UI, valid move checking, check/checkmate
detection, and optional AI gameplay. Exclusions include network
multiplayer and 3D graphics.  
• Technical Overview: The project was developed in C++ using Visual
Studio Code and SFML for GUI. Stockfish was integrated as the AI engine.

# 4. Methodology {#methodology}

• Approach: The project was developed in iterative phases including
planning, design, implementation, and testing. GitHub was used for
version control.  
• Roles and Responsibilities:  
- Abdullah: GUI and chess rules with logic.  
- Hassan Ahmed: Logic with AI integration.

\- Aazmeer: Chess rules and logic implementation.

# 5. Project Implementation {#project-implementation}

• Design and Structure: The project is organized into classes for each
chess piece, with a central Game class controlling the flow.
Encapsulation and polymorphism ensure modularity and reusability.  
• Functionalities Developed: Console-based gameplay, GUI rendering of
board and pieces, move validation, AI opponent (Stockfish).  
• Challenges Faced: Bishop's diagonal move validation initially caused
glitches. The issue was traced to improper path-checking and was
resolved by correctly verifying that all intermediate squares are empty
during a diagonal move.

# 6. Results {#results}

• Project Outcomes: A working chess game in C++ demonstrating
object-oriented design.  
• Screenshots and Illustrations: (Add manually as needed, e.g., from
SFML GUI interface and console view.)  
• Testing and Validation: Manual testing was conducted for all pieces'
movement, capturing rules, and game end conditions like checkmate and
stalemate.

# 7. Conclusion {#conclusion}

• Summary of Findings: The team created a chess game that applies core
OOP concepts, solves logical challenges, and simulates realistic chess
behavior.  
• Final Remarks: The project not only met the proposed objectives but
also improved the team's coding discipline, debugging skills, and
understanding of C++ object-oriented design.
