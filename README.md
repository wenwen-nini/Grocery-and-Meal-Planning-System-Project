# Grocery-and-Meal-Planning-System-Project
A system made for the final project as a requirement on Computer Programming 1 and Data Strucutres and Algorithm.  This system ensures grocery list and meal plans organized in efficient way. 

## Features
1. **Grocery List Functionality﻿** - This feature allows user to:

	1.1 **Add Grocery Item** - Set up the instructions for the user to input the grocery item's	description such as name, category, and expiration date. The function reads the data	and the expiration date would be calculated to get the difference between current date and expiration date. Lastly, the datas would be saved at the object or structure queue. 

	1.2 **Remove Grocery Item** - The datas saved from the queue container will read by the system and remove the first input

	1.3 **View Grocery Item** - The system will show all the grocery items currently stored in the queue, displaying their names, category, and days until expiration. If the queue is empty, the user will be notify. 

2. **Meal Planning Functionality** - This feature allows the user to:

	2.1 **Add Meal Plan** - Create a meal plan for a specific day, including breakfast, lunch,	dinner, and snacks

	2.2 **Remove Meal** - The system will read the datas from the meal queue and remove the first input. 

	2.3 **View Meal Plans** - Display all scheduled meal plans in chronological order

3. **Expiration Tracking** - This system will:
- Highlight items approaching expiration (e.g., within 3 days) 
- Provide warnings when adding items that will expire before similar existing item (FIFO principle) 
4. **File Handling** - Implement persistent storage by:
- Saving grocery items to "groceries.txt"
- Saving meal plans to "mealplan.txt"
- Loading these files when the program starts. 
5. **Searching and Filter Functions** - Allows users to:
- Search for items by name
- Filter items by expiration timeframe (e.g., expiring this week)

## NOTE
*This project had minor issues and only runs on latest version of c++. It is a console-based program only without gui and database as this serve a foundation for practicing backend.*
*It is optional for programmers to further enhance the system and solve the minor issues.*
