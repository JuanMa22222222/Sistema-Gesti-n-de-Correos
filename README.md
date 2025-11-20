# 📬 Sistema de Gestión de Correos
**Versión:** 1.0  
**Autor:** Santiago Herrera, Juan Bohorquez y Julian Quintero
**Fecha:** 19 de noviembre de 2025  

---

## 📘 Descripción General
El **Sistema de Gestión de Correos** es una herramienta diseñada para cargar, limpiar, normalizar, indexar y buscar direcciones de correo electrónico almacenadas en un archivo de texto.

Su principal objetivo es permitir una **búsqueda eficiente** mediante el uso de un **índice hash (`unordered_map`)**, lo cual permite localizar direcciones rápidamente y ofrecer **sugerencias basadas en coincidencias parciales** cuando un correo no existe exactamente.

Este programa se ejecuta en consola e incluye resaltado de texto mediante códigos ANSI para mejorar la experiencia del usuario.

---

## 🚀 Funcionalidades Principales

### ✔️ Carga y limpieza de datos
- Lee líneas desde un archivo `.txt`.
- Elimina espacios innecesarios.
- Normaliza todos los correos a minúsculas.

### ✔️ Indexación eficiente
- Construye un índice hash (`unordered_map`) donde:
  - **clave:** correo limpio  
  - **valor:** versión original del archivo  
- Permite búsquedas en tiempo constante promedio (O(1)).

### ✔️ Búsqueda de correos
- Búsqueda exacta.
- Sugerencias basadas en coincidencias parciales.
- Manejo de casos donde no hay coincidencias.

### ✔️ Interfaz interactiva en consola
- Permite al usuario buscar múltiples correos.
- Comando `exit` para terminar la ejecución.

---

## 🛠️ Tecnologías / Librerías utilizadas
- **C++17**
- `unordered_map`
- `vector`
- `fstream`
- `sstream`
- Colores ANSI para la interfaz.

---

## 📁 Estructura del Proyecto

