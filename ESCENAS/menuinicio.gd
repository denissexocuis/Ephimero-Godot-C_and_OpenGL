extends Control

@onready var player = $"/root/Mundo/TOMOE"

func _on_salir_pressed() -> void:
	get_tree().quit()

func _on_nueva_pressed() -> void:
	get_tree().change_scene_to_file("res://ESCENAS/Mundo.tscn")

func _on_opciones_pressed() -> void:
	get_tree().change_scene_to_file("res://ESCENAS/MENUOPCIONES.tscn")

func _on_acerca_pressed() -> void:
	get_tree().change_scene_to_file("res://ESCENAS/ABOUT.tscn")

func _on_cargar_pressed() -> void:
	# Intentar cargar el juego guardado antes de cambiar de escena
	if load_game():
		# Si el juego se carga correctamente, cambiar a la escena del juego
		get_tree().change_scene_to_file("res://ESCENAS/Mundo.tscn")
	else:
		print("No se encontró un archivo guardado.")


func load_game() -> bool:
	var config = ConfigFile.new()
	var save_path = "user://savegame.cfg"
	
	# Verificar si el archivo de guardado existe
	if config.load(save_path) == OK:
		# Cargar los datos del archivo
		var player_position = config.get_value("player", "position", Vector2.ZERO)
		
		# Restaurar los datos en el jugador
		player.position = player_position
	return true
