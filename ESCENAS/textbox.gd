extends CanvasLayer

# Variables
@export var dialog_lines: Array = []  # Lista de líneas de diálogo
@export var typing_speed: float = 0.05  # Velocidad de escritura (en segundos)
var current_line_index: int = 0  # Índice de la línea actual
var is_typing: bool = false  # Si el texto está siendo escrito actualmente

# Referencias a nodos
@onready var label = $TextboxContainer/MarginContainer/HBoxContainer/Label
@onready var star = $TextboxContainer/MarginContainer/HBoxContainer/Star
@onready var end = $TextboxContainer/MarginContainer/HBoxContainer/End

func _ready():
	# Inicializa el diálogo si hay líneas configuradas
	if dialog_lines.size() > 0:
		show_line()
	else:
		hide_textbox()

func show_line():
	if current_line_index < dialog_lines.size():
		var line = dialog_lines[current_line_index]
		label.text = ""  # Limpia el texto
		is_typing = true
		star.visible = true  # Muestra el indicador "Star" (si es necesario)
		end.visible = false  # Oculta el indicador "End" durante la escritura
		start_typing(line)
	else:
		hide_textbox()

func hide_textbox():
	# Oculta todo el contenedor del textbox
	$TextboxContainer.visible = false

func start_typing(line: String):
	# Escribe carácter por carácter
	is_typing = true
	label.text = ""  # Limpia el texto actual
	var char_index = 0
	while char_index < line.length():
		label.text += line[char_index]
		await get_tree().create_timer(typing_speed).timeout
		char_index += 1
	is_typing = false
	end.visible = true  # Muestra el indicador "End" al terminar de escribir

func _input(event):
	# Detecta si el jugador avanza el texto
	if event.is_action_pressed("ui_accept") and not is_typing:
		current_line_index += 1
		show_line()
