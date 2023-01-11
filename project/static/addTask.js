import { deleteTaskListener } from './deleteTask.js';
import { checkTaskListener } from './checkTask.js';

let current = 0;

document.querySelector('#create-task').addEventListener('submit', addTask);

function addTask(e) {
	e.preventDefault();
	current = parseInt(document.querySelector('#current-tasks').value);
	if (current < 8)
	{
		let task = document.querySelector('#task-content').value;
		let params = "task=" + task;

		let xhr = new XMLHttpRequest();
		xhr.open('POST', '/create_task', true);
		xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');

		xhr.onload = function() {
			let div = document.createElement("div");
			let checkbox = document.createElement("input");
			let label = document.createElement("label");
			let del = document.createElement("i");

			div.classList.add("form-check", "h4");
			checkbox.classList.add("form-check-input", "bg-success", "check-task");
			checkbox.setAttribute("type", "checkbox");
			checkbox.setAttribute("name", "task");
			checkbox.setAttribute("value", task);
			label.classList.add("form-check-label", "text-info");
			label.innerHTML = task;
			del.classList.add("bx", "bx-calendar-x", "text-danger", "delete-button");
			del.setAttribute("type", "button");
			del.innerHTML = '';

			div.appendChild(checkbox);
			div.appendChild(label);
			div.appendChild(del);

			document.querySelector('#today-tasks').appendChild(div);
			document.querySelector('#create-task').reset();
			deleteTaskListener();
			checkTaskListener();

			current += 1;
		}

		xhr.send(params);
	}
	else
	{
		alert("Maximum 8 tasks per day!");
	}

}