extends Area2D

@export var text_key: String = ""  # Usado si deseas mostrar texto
var area_active: bool = false

# Ruta del programa en C que deseas ejecutar
# var command = "/home/denissexocuis/Documentos/folder-master/master/Graficacion_Computadora/Trabajos/Éphimero. Game/Minigames/MG1"
# var arguments = []  # Si necesitas argumentos adicionales

@onready var player = $"/root/Mundo/TOMOE"  # Asumiendo que TOMOE es el jugador, y está en la escena "Mundo"

# Esta función se llama cuando el jugador entra en el área
func _on_body_entered(body):
	if body == player:  # Verifica que el cuerpo que entra sea el jugador
		area_active = true  # El área está activa

func _input(event):
	if area_active and event.is_action_pressed("ui_accept"):
		open_c_program()

func open_c_program():
	# convierte la ruta de Godot a una ruta absoluta del sistema de archivos
	var texture_path = ProjectSettings.globalize_path("res://TEXTURAS/GAME2.png") 
	
	# ruta del programa
	var command = "/home/denissexocuis/Documentos/folder-master/master/Graficacion_Computadora/Trabajos/Éphimero. Game/Minigames/MG2"
	
	# ruta de la textura
	var arguments = [texture_path]  # El programa C debe esperar este argumento
	
	# ejecuta el programa en c
	var output = []  
	var result = OS.execute(command, arguments, output, false, false)  
	
	if result == 0:
		print("El programa en C se ejecutó correctamente.")
	else:
		print("Hubo un error al ejecutar el programa en C.")
	
	# y después, cambiar escena
