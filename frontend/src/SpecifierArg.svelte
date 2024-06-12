<script>
    export let specifier = "";
    let arg = "";
</script>

<div>
    <label>
        <span class="label-text text-mono">
            {specifier}
        </span>
        <input bind:value={arg} class="format-arg"/>
    </label>
</div>