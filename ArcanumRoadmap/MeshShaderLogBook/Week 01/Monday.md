Today I spent time to make the structure between engine to editor/standalone more robust. I want each layer of the engine to handle specific things. Like framework should have all the managers while the game should only have the logic.

Had to do a lot of trial and error to try making something akin to dependency injection.