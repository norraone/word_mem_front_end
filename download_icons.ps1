$icons = @{
    "app_icon" = "https://raw.githubusercontent.com/google/material-design-icons/master/png/action/book/materialicons/48dp/2x/baseline_book_black_48dp.png"
    "light_theme" = "https://raw.githubusercontent.com/google/material-design-icons/master/png/action/light_mode/materialicons/24dp/2x/baseline_light_mode_black_24dp.png"
    "dark_theme" = "https://raw.githubusercontent.com/google/material-design-icons/master/png/action/dark_mode/materialicons/24dp/2x/baseline_dark_mode_black_24dp.png"
    "login" = "https://raw.githubusercontent.com/google/material-design-icons/master/png/action/login/materialicons/24dp/2x/baseline_login_black_24dp.png"
    "register" = "https://raw.githubusercontent.com/google/material-design-icons/master/png/social/person_add/materialicons/24dp/2x/baseline_person_add_black_24dp.png"
    "add" = "https://raw.githubusercontent.com/google/material-design-icons/master/png/content/add/materialicons/24dp/2x/baseline_add_black_24dp.png"
    "edit" = "https://raw.githubusercontent.com/google/material-design-icons/master/png/image/edit/materialicons/24dp/2x/baseline_edit_black_24dp.png"
    "delete" = "https://raw.githubusercontent.com/google/material-design-icons/master/png/action/delete/materialicons/24dp/2x/baseline_delete_black_24dp.png"
    "help" = "https://raw.githubusercontent.com/google/material-design-icons/master/png/action/help/materialicons/24dp/2x/baseline_help_black_24dp.png"
}

foreach ($icon in $icons.GetEnumerator()) {
    $outFile = "icons/$($icon.Key).png"
    Write-Host "Downloading $($icon.Key) to $outFile"
    Invoke-WebRequest -Uri $icon.Value -OutFile $outFile
}
