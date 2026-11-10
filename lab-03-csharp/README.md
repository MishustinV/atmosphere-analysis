Нужен .NET SDK. Если `dotnet --version` показывает не 8.x, поменяйте `TargetFramework` в `Atmosphere.csproj` на свою версию (например, `net9.0`).

Сборка и запуск из корня репозитория:

```
dotnet build csharp -c Release
dotnet run --project csharp -c Release
```

Запускать лучше в конфигурации Release: в Debug сборщик мусора может дольше удерживать локальные объекты.

Если в консоли Windows вместо русских букв вопросительные знаки, выполните `chcp 65001`.
