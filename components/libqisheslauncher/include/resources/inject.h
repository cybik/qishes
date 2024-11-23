//
// Created by cybik on 22-09-04.
//

#ifndef QAGL_INJECT_H
#define QAGL_INJECT_H

#define QAGL_INJECT_SETTINGS \
"(function() {\n" \
    "img_idle = document.createElement('img'); img_idle.src = '%1';\n"\
    "img_idle.className = 'unactive'; img_idle.alt = 'icon';\n"\
    "img_hovr = document.createElement('img'); img_hovr.src = '%2';\n"\
    "img_hovr.className = 'active'; img_hovr.alt = 'icon';\n"\
    "settings = document.createElement('div'); settings.id = 'settings';\n"\
    "settings.appendChild(img_idle); settings.appendChild(img_hovr);\n"\
    "document.body.appendChild(settings);\n"\
    "const t = document.getElementById(\"settings\");\n"\
    "t.onclick = () => { t == null || location.assign('%3'); },\n"\
    "t.onmouseenter = () => { t == null || t.classList.add(\"hovered\"); },\n"\
    "t.onmouseleave = () => { t == null || t.classList.remove(\"hovered\"); }"\
"})()"
#define QAGL_INJECT_STYLESHEET \
"(function() {\n" \
    "css = document.createElement('style'); css.type = 'text/css'; css.id = '%1';\n"\
    "css.textContent = `%2`;\n"\
    "document.head.appendChild(css);\n"\
"})()"
#endif //QAGL_INJECT_H
