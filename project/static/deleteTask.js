export function deleteTaskListener() {
	let deleteButtons = document.querySelectorAll('.delete-button');

	deleteButtons.forEach(function (btn) {
		btn.addEventListener('click', function () {

			if (confirm('Are you sure you want to give up the task?')) {
				console.log('Alright then');
			}

			else {
				console.log('Great!');
				return ;
			}

			let parentElement = btn.parentElement;

			let task = parentElement.querySelector('label').innerText;
			let params = "task=" + task;

			let xhr = new XMLHttpRequest();
			xhr.open('POST', '/delete_task', true);
			xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');

			xhr.onload = function() {
				parentElement.remove();
			}

			xhr.send(params);
		});
	});
}

deleteTaskListener();