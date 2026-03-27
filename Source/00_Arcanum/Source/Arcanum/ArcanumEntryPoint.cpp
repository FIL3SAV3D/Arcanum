#include <iostream>

#include <Editor/Editor.h>

int main(int /*NumArgs*/, char** /*ppArgs*/)
{
    system("cls");

    try
    {
        Editor* editor = new Editor();

        editor->Create();

        // Game Loop
        //editor->OnEnter();

        while (!editor->IsQuitting())
        {
            editor->Run();
        }

        //editor->OnExit();
        // Game Loop

        editor->Destroy();

        delete editor;
    }
    catch (const std::exception& e)
    {
        std::printf(e.what());
        system("pause");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}