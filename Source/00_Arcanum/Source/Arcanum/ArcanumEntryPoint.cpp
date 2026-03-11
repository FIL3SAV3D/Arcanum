#include <iostream>

#include <Editor/Editor.h>

int main(int /*NumArgs*/, char** /*ppArgs*/)
{
    system("cls");

    try
    {
        Editor* editor = new Editor();

        editor->Create();

        while (!editor->IsQuitting())
        {
            editor->Run();
        }

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