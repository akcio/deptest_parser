# deptest_parser
Используется для расшифровки файлов формата *deptest, используемых в ПО для тестирования студентов.
Программа может расшифровать вопросы с ответами из файла, затем сохранить их в формат INI или HTML.
# Использование

Генерация в html ``` reEncoding --if=input.deptest --of=output.html -m html ```

Генерация в ini ``` reEncoding --if=input.deptest --of=output.html -m ini ```

Расшифровка строки ``` reEncoding -s %Your String% ```

Чтобы смотреть вопросы вместе с прикрепленными к ним картинками нужно положить сгенерированный html в папку с картинками и открыть его браузером

# Примеры
В папке examples находится пример тестового файла, папка с картинками и в ней сгенерированный html файл
