{
    1. Некоторый   текст   состоит   из   нескольких   частей,
    записанных  в  отдельных  файлах.  Имена  этих  файлов и общий
    заголовок текста указаны в отдельном  файле.  Создать  файл  с
    полным  текстом.  Заголовок должен содержаться в центре первой
    строки (6).
    
    2. Автор - Никита Черемин
    3. Среда выполнения - Free Pascal Compiler version 3.2.0 [2020/06/22] for x86_64
    4. Формат входного файла:
            (%Заголовок%)
            (%Путь к первому файлу%)
            (%Путь ко второму файлу%)
            ...

       Запуск программы:
            main.exe (%Путь для входного файла%) (%Путь для сохранения получившегося текста%) 
}
PROGRAM JoinFiles(INPUT, OUTPUT);
{$MODE OBJFPC} 
USES Sysutils;
CONST
    NUMBER_OF_PARAMS = 2;
VAR
    InputFilePath, OutputFilePath, NewFileTitle, CurrFileName: UnicodeString;
    InputFile, OutputFile, CurrFile: TextFile;

PROCEDURE CopyFile(VAR InputFile, OutputFile: TextFile);
VAR
    Ch: WideChar;
BEGIN
    WHILE NOT EOF(InputFile)
    DO
        BEGIN
            WHILE NOT EOLN(InputFile)
            DO
                BEGIN
                    READ(InputFile, Ch);
                    WRITE(OutputFile, Ch);
                END;
            READLN(InputFile);
            WRITELN(OutputFile)
        END
END;

FUNCTION GetParams(VAR InputFilePath, OutputFilePath: UnicodeString): BOOLEAN;
{ Функция получает переданные в программу параметры. Если получение 
  прошло неудачно, возвращается FALSE, иначе - TRUE }
BEGIN
    IF paramCount() <> NUMBER_OF_PARAMS
    THEN
        GetParams := FALSE
    ELSE
        BEGIN
            InputFilePath := UnicodeString(paramStr(1));
            OutputFilePath := UnicodeString(paramStr(2));
            GetParams := TRUE
        END
END;

FUNCTION TryCreateFile(VAR FileForCreating: TEXT; VAR FileNameForCreating: UnicodeString): BOOLEAN;
{ Функция пытается создать файл по пути из FileNameForCreating и связать его 
  с файлом FileForCreating. Если операция прошла удачно, возвращается TRUE,
  иначе - FALSE. }
VAR
    FuncResult: BOOLEAN;
BEGIN
    FuncResult := TRUE;
    TRY
        ASSIGN(FileForCreating, FileNameForCreating);
        REWRITE(FileForCreating);
    EXCEPT
        FuncResult := FALSE;
    END;
    TryCreateFile := FuncResult
END;

BEGIN
    IF NOT GetParams(InputFilePath, OutputFilePath)
    THEN
        WRITELN('Некорректные входные параметры')
    ELSE IF NOT FileExists(InputFilePath)
    THEN
        WRITELN('Файл "', InputFilePath ,'" не найден')
    ELSE IF NOT TryCreateFile(OutputFile, OutputFilePath)
    THEN
        WRITELN('Не удалось создать файл "', OutputFilePath ,'"')
    ELSE
        BEGIN
            ASSIGN(InputFile, InputFilePath);
            RESET(InputFile);
            IF NOT EOF(InputFile)
            THEN
                BEGIN
                    READLN(InputFile, NewFileTitle);
                    WRITELN(OutputFile, NewFileTitle);
                    WRITELN(OutputFile)
                END;
            WHILE NOT EOF(InputFile)
            DO
                BEGIN
                    READLN(InputFile, CurrFileName);
                    IF NOT FileExists(CurrFileName)
                    THEN
                        WRITELN('Файл "', CurrFileName ,'" не найден')
                    ELSE
                        BEGIN
                            ASSIGN(CurrFile, CurrFileName);
                            RESET(CurrFile);
                            CopyFile(CurrFile, OutputFile);
                        END
                END;
            WRITELN('Объединенный файл был создан и находится по пути "', OutputFilePath ,'"');
            CLOSE(InputFile);
            CLOSE(OutputFile)
        END
END.
