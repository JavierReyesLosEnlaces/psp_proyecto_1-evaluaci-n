# psp_proyecto_1ev
Repositorio para trabajar en el proyecto de la 1ª evaluación de Programación de Servicios y Procesos
-----------------------------------------------------------------------------------------------------
Comando para clonar vuestra propia rama:

	git clone --branch tu_rama ruta_https_del_repositorio

Para ver el estado de nuestro repositorio os recomiendo que useis el siguiente comando:

	gitk --all

# Cómo trabajar desde tu rama y subirlo a main

Antes de trabajar, te sitúas en (main) y haces un git pull origin main:

	git checkout main
	git pull origin main

	Te traes los cambios más recientes de main.

Luego cambias a (tu_rama) y haces un git merge main:

	git checkout tu_rama
	git merge main

	Fusionas los cambios de tu rama main con tu rama.

Trabajas, haces tus commits y haces git push origin tu_rama:
	
	git add .
	git commit -m "Mensaje"
	git push origin tu_rama

	Pusheas los cambios a tu rama para tenerlo guardado en remoto.

Te sitúas en (main) y haces un git merge tu_rama:

	git checkout main
	git merge tu_rama

	Fusiona los cambios de (tu_rama) con (main).

Haces un commit desde main y pusheas los cambios:

	git commit -m "Cambios realizados en main"
	git push origin main
