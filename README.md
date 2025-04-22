[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/FlzTjSEb)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-2e0aaae1b6195c2367325f4f02e2d04e9abb55f0b24a779b69b11b9e10269abc.svg)](https://classroom.github.com/online_ide?assignment_repo_id=18449762&assignment_repo_type=AssignmentRepo)
# Assignment 4 - Airport_Trains

Jasey Chanders 
March 4th, 2025

Airport Trains using interrupts and pwm

See "state machine.jpg" for state machine layout

Review Feedback: 
- "Main Loop Starting" printf appears in both Debug and Release. - fixed
- Debug message for emergency state transitions doesn't follow spec. - fixed
- The whole state machine is in the SysTick interrupt handler. - semi fixed though a challenging fix without breaking things so I didn't remove as much as I'd like
- No state for emergeny. - It does exist just is handled separately and is a little less obvious how.
