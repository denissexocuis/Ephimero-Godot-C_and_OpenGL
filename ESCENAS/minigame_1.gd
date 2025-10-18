extends Area2D

# Variable para controlar si el minijuego está en ejecución
var c_program_running = false

# Función que se llama cuando un cuerpo entra en el área
func _on_Puerta_body_entered(body):
	# Verificamos que sea el jugador (asegúrate de que el nombre del personaje sea "Player")
	if body.name == "Player" and !c_program_running:
		print("El personaje llegó a la puerta")
		_start_c_program()  # Llamamos a la función que abre el minijuego

# Función para iniciar el programa en C
func _start_c_program():
	print("Ejecutando minijuego More_Block_Puzzle...")

	# Ruta al ejecutable del programa en C (cambia esta ruta a la correcta)
	var program_path = "/home/denissexocuis/Documentos/folder-master/master/Graficacion_Computadora/Trabajos/Éphimero. Game/Minigames/More_Block_Puzzle.c"  # Ejemplo: "/home/usuario/More_Block_Puzzle"
	var args = []  # Si el programa en C necesita argumentos, agréguelos aquí

	# Ejecutamos el programa en C
	var exit_code = OS.execute(program_path, args, true)  # El tercer parámetro es un bool, 'true' para ejecución bloqueante
	
	# Marcamos que el minijuego está en ejecución
	c_program_running = true

	# Esperamos que el minijuego termine
	if exit_code == 0:
		print("Minijuego completado con éxito")
		_allow_scene_change()  # Si se completó con éxito, cambiamos de escena
	else:
		print("El minijuego no se completó o falló")
		c_program_running = false  # Si no se completó, no permitimos el cambio de escena

# Función para permitir cambiar a la siguiente escena
func _allow_scene_change():
	print("Cambiando a la siguiente escena...")
	# Cambia la escena a la siguiente
	get_tree().change_scene("res://ruta/a/la_siguiente_escena.tscn")  # Cambia esta ruta por la escena correcta
