extends CharacterBody2D

''' Primero definimos las variables de movimiento '''
@export var speed = 200
@export var jump_speed = 270
# Para que el personaje gire de acuerdo a su movimiento (<- ->)
var is_facing_right = true
# para obtener la variable default de gravedad que tiene godot y hacer la física del salto
var gravity = ProjectSettings.get_setting("physics/2d/default_gravity")
# para obtener la referencia antes de que el juego empiece
@onready var animated_sprite = $TOMOE

''' Funciones de evento para el movimiento '''
# Aquí se maneja la lógica de la física del juego.
# Se llama a la clase `Input` que requiere 2 acciones, una positiva y una negativa.

func _physics_process(delta):
	# esto es para el salto
	if Input.is_action_just_pressed("ui_up"):
		velocity.y = -jump_speed
	velocity.y += gravity * delta

	# moverse en x y girar, también para las animaciones
	move_x()
	flip()
	update_animations()
	move_and_slide()

func update_animations():
	#logica de animaciones
	if velocity.x:
		animated_sprite.play("Caminar") #poniendo la animacion animar que diseñé ^^
	else:
		animated_sprite.play("Frente")

func move_x():
	var input_axis = Input.get_axis("ui_left", "ui_right")
	velocity.x = input_axis * speed

func flip():
	# Si está a la derecha y se aprieta izquierda,
	# o si está a la izquierda y se aprieta derecha:
	if (is_facing_right and velocity.x < 0) or (not is_facing_right and velocity.x > 0):
		scale.x *= -1
		is_facing_right = not is_facing_right
