let checkbox = document.querySelector("#dark-theme");

checkbox.addEventListener('change', function (e) {
	if (e.target.checked)
	{
		setDarkTheme();
	}
	else
	{
		setDefaultTheme();
	}
})

function setDarkTheme() {
	document.body.style.backgroundColor = "#990066";
	document.querySelector("#nav-menu").style.backgroundColor = "#996633";

	checkbox.setAttribute('checked', '');
	sessionStorage.setItem('theme', 'dark');
}

function setDefaultTheme() {
	document.body.style.backgroundColor = "#ffccff";
	document.querySelector("#nav-menu").style.backgroundColor = "#ffff66";

	checkbox.removeAttribute('checked');
	sessionStorage.setItem('theme', 'default');
}

if (sessionStorage.getItem('theme') === 'dark')
{
	setDarkTheme();
}